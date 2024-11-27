#include "game.h"
#include <algorithm>  

Game::Game(std::shared_ptr<Queue<game_state_t>> gameStateQueue, std::shared_ptr<Queue<uint8_t>> commandQueue, Client& client)
    : gameStateQueue(gameStateQueue),
      commandQueue(commandQueue),
      gWindow(nullptr, SDL_DestroyWindow),
      gRenderer(nullptr, SDL_DestroyRenderer),
      client(client)
{
    gameState = gameStateQueue->pop();

    ducks.resize(MAX_DUCKS);
    platforms.resize(MAX_PLATFORMS);
    spawns.resize(MAX_SPAWN_PLACES);
    droppedWeapons.resize(MAX_ITEMS);
    droppedArmors.resize(MAX_ITEMS);
    projectiles.resize(MAX_PROJECTILES);
    boxes.resize(MAX_BOXES);
    bananas.resize(MAX_BOXES);
}

bool Game::loadMedia() {
    bool success = true;
    
    try {
        background = std::make_unique<LTexture>(gRenderer.get());
        if (!background || !background->loadFromFile("client_src/forest.png")) {
            throw std::runtime_error("Failed to load background texture");
        }
        
        gFont.reset(TTF_OpenFont("client_src/barcadesemital.ttf", 28));
        if (!gFont) {
            throw std::runtime_error("Failed to load font");
        }

        initializeGameObjects();
        
    } catch (const std::exception& e) {
        std::cerr << "Error loading media: " << e.what() << std::endl;
        success = false;
    }
    
    return success;
}

void Game::initializeGameObjects() {
    if (gameState.level.num_ducks > MAX_DUCKS ||
        gameState.level.num_platforms > MAX_PLATFORMS ||
        gameState.level.num_spawn_places > MAX_SPAWN_PLACES ||
        gameState.level.num_boxes > MAX_BOXES) {
        throw std::runtime_error("Game state exceeds maximum allowed objects");
    }

    ducks.clear();
    platforms.clear();
    spawns.clear();
    boxes.clear();
    
    ducks.resize(gameState.level.num_ducks);
    platforms.resize(gameState.level.num_platforms);
    spawns.resize(gameState.level.num_spawn_places);
    boxes.resize(gameState.level.num_boxes);
}

#include <chrono>

const std::chrono::milliseconds COMMAND_INTERVAL(50); 
const std::chrono::milliseconds SHOOT_INTERVAL(200); 
std::chrono::steady_clock::time_point lastCommandTime = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point lastShootTime = std::chrono::steady_clock::now();

bool leftPressed = false;
bool rightPressed = false;
bool shootPressed = false;

bool Game::processEvents() {
    SDL_Event e;
    bool eventDetected = false;

    while (SDL_PollEvent(&e) != 0) {
        eventDetected = true;
        if (e.type == SDL_QUIT) {
            sendCommand(LEAVE_MATCH);
            return true;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP: sendCommand(JUMP); break;
                case SDLK_DOWN: sendCommand(FLOOR); break;
                case SDLK_LEFT: leftPressed = true; break;
                case SDLK_RIGHT: rightPressed = true; break;
                case SDLK_RSHIFT: sendCommand(TAKE_WEAPON); break;
                case SDLK_LCTRL: shootPressed = true; break;
                case SDLK_F1: sendCommand(INFINIT_AMMO); break;
                case SDLK_F2: sendCommand(PICK_ANY_WEAPON); break;
                case SDLK_F3: sendCommand(CHESTPLATE_ARMOR); break;
                case SDLK_F4: sendCommand(HELMET_ARMOR); break;
                case SDLK_1: sendCommand(GRENADE_WEAPON); break;
                case SDLK_2: sendCommand(BANANA_WEAPON); break;
                case SDLK_3: sendCommand(PEWPEWLASER_WEAPON); break;
                case SDLK_4: sendCommand(LASERRIFLE_WEAPON); break;
                case SDLK_5: sendCommand(AK_47_WEAPON); break;
                case SDLK_6: sendCommand(DARTGUN_WEAPON); break;
                case SDLK_7: sendCommand(COWBOY_WEAPON); break;
                case SDLK_8: sendCommand(MAGNUM_WEAPON); break;
                case SDLK_9: sendCommand(SHOTGUN_WEAPON); break;
                case SDLK_0: sendCommand(SNIPER_WEAPON); break;
                case SDLK_g: sendCommand(RESTART_MATCH); break;
                default: break;
            }
        } else if (e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym) {
                case SDLK_LEFT: leftPressed = false; break;
                case SDLK_RIGHT: rightPressed = false; break;
                case SDLK_LCTRL: shootPressed = false; break;
                default: break;
            }
        }
    }

    auto currentTime = std::chrono::steady_clock::now();
    if (currentTime - lastCommandTime >= COMMAND_INTERVAL) {
        if (leftPressed) sendCommand(MOVE_LEFT);
        if (rightPressed) sendCommand(MOVE_RIGHT);
        lastCommandTime = currentTime;
    }

    if (shootPressed && currentTime - lastShootTime >= SHOOT_INTERVAL) {
        sendCommand(SHOOT);
        lastShootTime = currentTime;
    }

    if (!eventDetected) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return false;
}

void Game::sendCommand(const uint8_t command){
    commandQueue->try_push(command);
}

bool Game::init() {
    std::lock_guard<std::mutex> lock(sdl_mutex);
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_Window* windowPtr = SDL_CreateWindow("Duck", 
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        SCREEN_WIDTH, 
        SCREEN_HEIGHT, 
        SDL_WINDOW_SHOWN);
    
    if (!windowPtr) {
        std::cerr << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    gWindow.reset(windowPtr);

    SDL_Renderer* rendererPtr = SDL_CreateRenderer(gWindow.get(), -1, 
        SDL_RENDERER_ACCELERATED);
    
    if (!rendererPtr) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        gWindow.reset();
        SDL_Quit();
        return false;
    }
    gRenderer.reset(rendererPtr);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        gRenderer.reset();
        gWindow.reset();
        SDL_Quit();
        return false;
    }

    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        gRenderer.reset();
        gWindow.reset();
        SDL_Quit();
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        gRenderer.reset();
        gWindow.reset();
        SDL_Quit();
        return false;
    }

    if (music.loadMusic("client_src/soundtrack/song1.mp3")) {
        music.setVolume(8);  
        music.play();
    }

    return true;
}

void Game::run()
{
    if (!init() || !loadMedia()) {
        printf("Failed to initialize game or load media.\n");
        return;
    }

    bool quit = false;
    auto next_frame = std::chrono::steady_clock::now();

    while (!quit && _keep_running) {
        quit = processEvents();
        SDL_SetRenderDrawColor(gRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer.get());


        while (gameStateQueue->try_pop(gameState)) {
            continue;
        }

        update(gameState);

        SDL_FPoint center = CalculatorManager::calculateCenter(ducks);
        float maxDistance = CalculatorManager::calculateMaxDistance(ducks);
        
        zoom.update(maxDistance);
        camera.update(center.x, center.y, zoom);

        render();

        SDL_RenderPresent(gRenderer.get());

        next_frame += std::chrono::milliseconds(static_cast<int>(FRAME_DURATION_MS));
        std::this_thread::sleep_until(next_frame);

        auto frame_end = std::chrono::steady_clock::now();
        if (frame_end > next_frame) {
            next_frame = frame_end;
        }
    }
    client.stop();
}

void Game::render() {

    std::lock_guard<std::mutex> lock(sdl_mutex);
    if (!gRenderer || !gWindow) {
        std::cerr << "Renderer is null" << std::endl;
        return;
    }
    
    SDL_SetRenderDrawColor(gRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer.get());

    if (background) {
        SDL_Rect scaleRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        background->render(0, 0, nullptr, &scaleRect, SDL_FLIP_NONE);
    }



    for (const auto& platform : platforms) {
        if (platform) platform->render(camera, zoom.getCurrentZoom());
    }

    for (const auto& box : boxes) {
        if (box && box->getState().health > 0) box->render(camera, zoom.getCurrentZoom());
    }

    for (const auto& duck : ducks) {
        if (duck && duck->isAlive()) duck->render(camera, zoom.getCurrentZoom());
    }

    for (const auto& spawn : spawns) {
        if (spawn) spawn->render(camera, zoom.getCurrentZoom());
    }

    for (const auto& weapon : droppedWeapons) {
        if (weapon && weapon->getState().type != NULL_WEAPON) weapon->render(weapon->getState().pos.x, weapon->getState().pos.y, false, camera, zoom.getCurrentZoom());
    }

    for (const auto& armor : droppedArmors) {
        if (armor && armor->getState().type != NULL_ARMOR) armor->render(armor->getState().pos.x, armor->getState().pos.y, false, armor->getState().type, camera, zoom.getCurrentZoom());
    }

    for (const auto& projectile : projectiles) {
        if (projectile) { 
            projectile->render(camera, zoom.getCurrentZoom());
        }
    }

    for (int i = 0; i < gameState.level.num_explosions; i++) {
        if (explotions[i]) {
            SDL_Rect explosionRect;
            explosionRect.w = 16 * 4;
            explosionRect.h = 16 * 3;
            explosionRect.x = gameState.level.explosions[i].x;
            explosionRect.y = gameState.level.explosions[i].y + 16;

            SDL_Point screenPos = camera.getScreenPosition(explosionRect.x, explosionRect.y, zoom.getCurrentZoom());
            SDL_Rect destRect = {
                screenPos.x,
                screenPos.y,
                static_cast<int>(explosionRect.w * zoom.getCurrentZoom()),
                static_cast<int>(explosionRect.h * zoom.getCurrentZoom())
            };

            explotions[i]->renderAnimation(destRect.x, destRect.y, destRect, false, true);
        }
    }

    for (const auto& banana : bananas) {
        if (banana) {
            banana->render(camera, zoom.getCurrentZoom());
        }
    }

    /*SDL_Rect msjRect;
    msjRect.w = 16 * 4;
    msjRect.h = 16 * 3;
    msjRect.x = 32*13-40;
    msjRect.y = 400 - 32*5;

    SDL_Point screenPos = camera.getScreenPosition(msjRect.x, msjRect.y, zoom.getCurrentZoom());
    SDL_Rect destRect = {
        screenPos.x,
        screenPos.y,
        static_cast<int>(msjRect.w * zoom.getCurrentZoom()),
        static_cast<int>(msjRect.h * zoom.getCurrentZoom())
    };*/

    SDL_Point screenMsjPos = camera.getScreenPosition(32*13-40, 400 - 32*5, zoom.getCurrentZoom());
    

    for (int i = 0; i < gameState.level.num_ducks; i++) {
        if (gameState.level.ducks[i].score >= gameState.winning_score) {
            SDL_Point screenMsjWinPos = camera.getScreenPosition(gameState.level.ducks[i].pos.x - 100, gameState.level.ducks[i].pos.y-10, zoom.getCurrentZoom());
            renderText("WINNER!!!!! now press 'G' to restart.",  screenMsjWinPos.x, screenMsjWinPos.y, 1);
            renderText("Close the window to quit.",  screenMsjPos.x + 30, screenMsjPos.y + 20, 1); 
        } else {
            SDL_Point screenMsjWinPos = camera.getScreenPosition(gameState.level.ducks[i].pos.x + 8, gameState.level.ducks[i].pos.y-2, zoom.getCurrentZoom());
            renderText("Player: " + std::to_string(gameState.level.ducks[i].id + 1),  screenMsjWinPos.x, screenMsjWinPos.y, 0);
        }
    }

    SDL_RenderPresent(gRenderer.get());
}

void Game::update(game_state_t gameState) {
    std::lock_guard<std::mutex> lock(sdl_mutex);
    for (size_t i = 0; i < ducks.size(); i++) {
        bool found = false;
        for (int j = 0; j < gameState.level.num_ducks; j++) {
            if (ducks[i] && ducks[i]->getId() == gameState.level.ducks[j].id) {
                ducks[i]->updateState(gameState.level.ducks[j]);
                found = true;
                break;
            }
        }
        if (!found) {
            ducks[i] = nullptr;
        }
    }

    ducks.resize(gameState.level.num_ducks);
    for (int i = 0; i < gameState.level.num_ducks; i++) {
        bool exists = false;
        for (const auto& duck : ducks) {
            if (duck && duck->getId() == gameState.level.ducks[i].id) {
                exists = true;
                break;
            }
        }
        
        if (!exists) {
            ducks[i] = std::make_unique<Duck>(
                gameState.level.ducks[i],
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                gRenderer.get()
            );
            if (!ducks[i]->loadTexture()) {
                std::cout << "Failed to load texture for new duck with ID " << gameState.level.ducks[i].id << std::endl;
            }
        }
    }

    explotions.resize(gameState.level.num_explosions);
    for (int i = 0; i < gameState.level.num_explosions; ++i) {
        if (gameState.level.explosions[i].x == 0 && gameState.level.explosions[i].y == 0) {
            explotions[i] = nullptr;
            continue;
        }
        if (!explotions[i]) {
            explotions[i] = std::make_unique<Animation>(5, 16, 16, gRenderer.get(), FIRE);
            explotions[i]->loadTexture("client_src/guns/fire1.png");
        }
    }

    bananas.resize(gameState.level.num_bananas);
    for (int i = 0; i < gameState.level.num_bananas; ++i) {
        if (!bananas[i]) {
            bananas[i] = std::make_unique<Banana>(gRenderer.get());
            bananas[i]->loadTexture();
            continue;
        }
        bananas[i]->updatePosition(gameState.level.bananas[i]);
    }

    platforms.resize(gameState.level.num_platforms);
    for (int i = 0; i < gameState.level.num_platforms; ++i) {
        if (platforms[i]) {
            platforms[i]->updateState(gameState.level.platforms[i]);
        }
        else { 
            platforms[i] = std::make_unique<Platform>(gameState.level.platforms[i], gRenderer.get());
            platforms[i]->loadTexture();
        }
    }

    spawns.resize(gameState.level.num_spawn_places);
    for (int i = 4; i < gameState.level.num_spawn_places; ++i) {
        if (spawns[i]) {
            spawns[i]->updateState(gameState.level.spawn_places[i]);
        }
        else {
            spawns[i] = std::make_unique<SpawnPlace>(gameState.level.spawn_places[i], gRenderer.get());
            spawns[i]->loadTexture();
        }
    }

    droppedWeapons.resize(gameState.level.num_dropped_weapons);
    for (int i = 0; i < gameState.level.num_dropped_weapons; ++i) {
        if (!droppedWeapons[i]) {
            droppedWeapons[i] = std::make_unique<Weapon>(gameState.level.dropped_weapons[i], gRenderer.get());
            droppedWeapons[i]->loadTexture();
        } 
        if (droppedWeapons[i]) {
            droppedWeapons[i]->updateState(gameState.level.dropped_weapons[i]);
        }
    }

    droppedArmors.resize(gameState.level.num_dropped_armors);
    for (int i = 0; i < gameState.level.num_dropped_armors; ++i) {
        if (!droppedArmors[i]) {
            droppedArmors[i] = std::make_unique<Armor>(gameState.level.dropped_armors[i], gRenderer.get());
            droppedArmors[i]->loadTexture();
        }
        if (droppedArmors[i]) {
            droppedArmors[i]->updateState(gameState.level.dropped_armors[i]);
        }
    }

    projectiles.resize(gameState.level.num_projectiles);
    for (int i = 0; i < gameState.level.num_projectiles; ++i) {
        if (!projectiles[i] && gameState.level.projectiles[i].is_active) {
            projectiles[i] = std::make_unique<Projectile>(gameState.level.projectiles[i], gRenderer.get());
            if (!projectiles[i]->loadTexture()) {
                std::cout << "Failed to load projectile texture" << std::endl;
            }
        }
            

        if (!gameState.level.projectiles[i].is_active) {
            projectiles[i] = nullptr;
            continue;
        }
        if (projectiles[i])
            projectiles[i]->updateState(gameState.level.projectiles[i]);
    }   

    

    boxes.resize(gameState.level.num_boxes);
    for (int i = 0; i < gameState.level.num_boxes; ++i) {            
        if (boxes[i] && gameState.level.boxes[i].health <= 0) {
            boxes[i] = nullptr;
            continue;
        } 
        if (boxes[i] && gameState.level.boxes[i].health > 0) {
            boxes[i]->updateState(gameState.level.boxes[i]);
        }
        else {
            boxes[i] = std::make_unique<Box>(gameState.level.boxes[i], gRenderer.get());
            boxes[i]->loadTexture();
        }
    }
}

void Game::renderText(const std::string& message, int x, int y, int color) {
    if (!gFont) {
        std::cerr << "Font not loaded, cannot render text." << std::endl;
        return;
    }
    SDL_Color textColor;
    if (color == 1) {
        textColor = { 255, 128, 0, 255 };
    } else {
        textColor = { 255, 255, 255, 255 };
    }
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont.get(), message.c_str(), textColor);

    if (!textSurface) {
        std::cerr << "Unable to create text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer.get(), textSurface);
    if (!textTexture) {
        std::cerr << "Unable to create text texture! SDL Error: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(textSurface);
        return;
    }
    if(color == 0) {
        textSurface->w = 90;
        textSurface->h = 30;
    }
    SDL_Rect renderQuad = {x, y, textSurface->w, textSurface->h};

    if (SDL_RenderCopy(gRenderer.get(), textTexture, nullptr, &renderQuad) != 0) {
        std::cerr << "Error rendering text! SDL Error: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


void Game::stop() {
    std::lock_guard<std::mutex> lock(sdl_mutex);
    if (!_keep_running) return;
    try {
        Thread::stop();
        
        ducks.clear();
        platforms.clear();
        spawns.clear();
        droppedWeapons.clear();
        droppedArmors.clear();
        projectiles.clear();
        boxes.clear();
        background.reset();

/*         if (gFont) {
            TTF_CloseFont(gFont.get());
            gFont.reset();  
        }
        
        
        if (gRenderer) {
            SDL_RenderClear(gRenderer.get());
            SDL_RenderPresent(gRenderer.get());
            gRenderer.reset();
        }
        
        if (gWindow) {
            SDL_DestroyWindow(gWindow.get());
            gWindow.reset();
        } */


        //TTF_Quit();
        //IMG_Quit();
        SDL_Quit();
        Mix_CloseAudio();

    } catch (const std::exception& e) {
        std::cerr << "Error during game shutdown: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error during game shutdown." << std::endl;
    }
}



Game::~Game() {
    try {
        stop();
    } catch (const std::exception& e) {
        std::cerr << "Error in Game destructor: " << e.what() << std::endl;
    }
    printf("Game destroyed.\n");
}
