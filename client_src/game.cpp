#include "game.h"
#include <algorithm>  // Añadir este include al principio del archivo

Game::Game(std::shared_ptr<Queue<game_state_t>> gameStateQueue, std::shared_ptr<Queue<uint8_t>> commandQueue, Client& client)
    : gameStateQueue(gameStateQueue),
      commandQueue(commandQueue),
      gWindow(nullptr, SDL_DestroyWindow),
      gRenderer(nullptr, SDL_DestroyRenderer),
      client(client)
{
    gameState = gameStateQueue->pop();
<<<<<<< HEAD
    ducks.reserve(MAX_DUCKS);
    platforms.reserve(MAX_PLATFORMS);
    spawns.reserve(MAX_SPAWN_PLACES);
    droppedWeapons.reserve(MAX_ITEMS);
    droppedArmors.reserve(MAX_ITEMS);
    projectiles.reserve(MAX_PROJECTILES);
    boxes.reserve(MAX_BOXES);
}


/* bool Game::loadMedia()
{
    bool charged = true;
    ducks.resize(gameState.level.num_ducks);  // Ajuste: asegurar el tamaño correcto del vector ducks
    for (int i = 0; i < gameState.level.num_ducks && charged; i++) {
        if (!ducks[i]) {
            ducks[i] = std::make_unique<Duck>(gameState.level.ducks[i], SCREEN_WIDTH, SCREEN_HEIGHT, gRenderer.get());
        }
        if (!ducks[i]->loadTexture()) {
            printf("Failed to load texture for duck %d.\n", i);
            charged = false;
        }
    }
    platforms.resize(gameState.level.num_platforms);  // Ajuste: asegurar el tamaño correcto del vector ducks
    for (int i = 0; i < gameState.level.num_platforms && charged; i++) {
        if (!platforms[i]) {
            platforms[i] = std::make_unique<Platform>(gameState.level.platforms[i], gRenderer.get());
        }
        if (!platforms[i]->loadTexture()) {
            printf("Failed to load texture for duck %d.\n", i);
            charged = false;
        }
    }
    spawns.resize(MAX_SPAWN_PLACES);
    std::cout << "Tamaño de spawns: " << static_cast<int>(gameState.level.num_spawn_places) << std::endl; 
    for (int i = 0; i < gameState.level.num_spawn_places && charged; i++) {
        if (gameState.level.spawn_places[i].weapon.type == NULL_WEAPON && gameState.level.spawn_places[i].armor.type == NULL_ARMOR) continue;
        if (!spawns[i]) {
            std::cout << "Creando nuevo spawn: " << static_cast<int>(gameState.level.spawn_places[i].weapon.type)  << std::endl;
            std::cout << "Creando nuevo spawn: " << static_cast<int>(gameState.level.spawn_places[i].armor.type)  << std::endl;
            spawns[i] = std::make_unique<SpawnPlace>(gameState.level.spawn_places[i], gRenderer.get());
        }
        if (!spawns[i]->loadTexture()) {
            printf("Failed to load texture for spawn %d.\n", i);
            charged = false;
        }
    }

    droppedWeapons.resize(MAX_ITEMS);
    std::cout << "Cambiando el tamaño de las cajas\n";
    droppedArmors.resize(MAX_ITEMS);
    std::cout << "Cajas reziseadas.\n";
    projectiles.resize(MAX_PROJECTILES);

    boxes.resize(MAX_BOXES);
    for (int i = 0; i < gameState.level.num_boxes && charged; i++) {
        if (!boxes[i]) {
            std::cout << "Cargando boxes\n";
            boxes[i] = std::make_unique<Box>(gameState.level.boxes[i], gRenderer.get());
        }
        if (!boxes[i]->loadTexture()) {
            printf("Failed to load texture for box %d.\n", i);
            charged = false;
        }
    }

    background = std::make_unique<LTexture>(gRenderer.get());

    if(!background->loadFromFile("client_src/forest.png")){
        charged = false;
    }

    return charged;
} */
=======
    ducks.resize(MAX_DUCKS);
    platforms.resize(MAX_PLATFORMS);
    spawns.resize(MAX_SPAWN_PLACES);
    droppedWeapons.resize(MAX_ITEMS);
    droppedArmors.resize(MAX_ITEMS);
    projectiles.resize(MAX_PROJECTILES);
    boxes.resize(MAX_BOXES);
}
>>>>>>> origin/Editor

bool Game::loadMedia() {
    bool success = true;
    
    try {
        background = std::make_unique<LTexture>(gRenderer.get());
        if (!background || !background->loadFromFile("client_src/forest.png")) {
            throw std::runtime_error("Failed to load background texture");
        }
        
        gFont.reset(TTF_OpenFont("client_src/Namaku.ttf", 28));
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
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Creación de la ventana con smart pointer
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

    // Creación del renderer con smart pointer
    SDL_Renderer* rendererPtr = SDL_CreateRenderer(gWindow.get(), -1, 
        SDL_RENDERER_ACCELERATED);
    
    if (!rendererPtr) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        gWindow.reset();
        SDL_Quit();
        return false;
    }
    gRenderer.reset(rendererPtr);

    // Inicialización de SDL_image
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        gRenderer.reset();
        gWindow.reset();
        SDL_Quit();
        return false;
    }

    if(TTF_Init() < 0) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        gRenderer.reset();
        gWindow.reset();
        SDL_Quit();
        return false;
    }

    return true;
}

void Game::run()
{
    if (!init() || !loadMedia()) {
        printf("Failed to initialize game or load media.\n");
        return;
    }

    //printf("Game initialized and media loaded successfully.\n");

    bool quit = false;
    auto next_frame = std::chrono::steady_clock::now();

    while (!quit && _keep_running) {
        quit = processEvents();
        std::cout << quit << std::endl;
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
    std::cout << "Saliendo de Game, llamando a stop" << std::endl;
    client.stop();
}

void Game::render() {
<<<<<<< HEAD

    if (!gRenderer) {
=======
    std::lock_guard<std::mutex> lock(sdl_mutex);
    if (!gRenderer || !gWindow) {
>>>>>>> origin/Editor
        std::cerr << "Renderer is null" << std::endl;
        return;
    }
    
    SDL_SetRenderDrawColor(gRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer.get());

<<<<<<< HEAD
    // Render background
    if (background) {
        SDL_Rect scaleRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        background->render(0, 0, nullptr, &scaleRect, SDL_FLIP_NONE);
    }
=======
    SDL_Rect scaleRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    background->render(0, 0, nullptr, &scaleRect, SDL_FLIP_NONE);
    /*SDL_Rect bgRect = camera.getBackgroundRect(
            background->getWidth(), 
            background->getHeight(), 
            zoom.getCurrentZoom()
        );
    background->render(bgRect.x, bgRect.y, NULL, &bgRect, SDL_FLIP_NONE);*/
>>>>>>> origin/Editor


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
            explosionRect.w = 16 * 4; // Ajusta estos valores según el tamaño deseado
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

    for (int i = 0; i < gameState.level.num_ducks; i++) {
        if (gameState.level.ducks[i].score >= gameState.winning_score) {
            renderText("WINNER!!!!! now press 'G' to restart.",  gameState.level.ducks[i].pos.x - 250, gameState.level.ducks[i].pos.y-30);
            renderText("Close the window to quit.",  (32*13-40)*zoom.getCurrentZoom(), (400 - 32*5)+zoom.getCurrentZoom()); 
        }
    }

    SDL_RenderPresent(gRenderer.get());
}

void Game::update(game_state_t gameState) {
    std::lock_guard<std::mutex> lock(sdl_mutex);
    // Primero actualizamos los patos existentes y removemos los que ya no están
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

    // Luego ajustamos el tamaño y agregamos los nuevos patos
    ducks.resize(gameState.level.num_ducks);
    // Finalmente, agregamos los patos que faltan
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
            std::cout << "Creando nuevo pato " << gameState.level.ducks[i].id << std::endl;
            if (!ducks[i]->loadTexture()) {
                std::cout << "Failed to load texture for new duck with ID " << gameState.level.ducks[i].id << std::endl;
            }
            std::cout << "New duck initialized with ID " << gameState.level.ducks[i].id << std::endl;
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

void Game::renderText(const std::string& message, int x, int y) {
    if (!gFont) {
        std::cerr << "Font not loaded, cannot render text." << std::endl;
        return;
    }
    // naranja
    SDL_Color textColor = { 255, 128, 0, 255 };
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

    SDL_Rect renderQuad = {x, y, textSurface->w, textSurface->h};

    // Renderizamos la textura del texto
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
        
        // Clear game-specific resources
        ducks.clear();
        platforms.clear();
        spawns.clear();
        droppedWeapons.clear();
        droppedArmors.clear();
        projectiles.clear();
        boxes.clear();
        background.reset();

        // Primero liberar la fuente antes de TTF_Quit
        if (gFont) {
            TTF_CloseFont(gFont.get());
            gFont.reset();  // Liberamos la fuente TTF primero
        }
        
        
        // Clear SDL-specific resources
        if (gRenderer) {
            SDL_RenderClear(gRenderer.get());
            SDL_RenderPresent(gRenderer.get());
            gRenderer.reset();
        }
        
        if (gWindow) {
            SDL_DestroyWindow(gWindow.get());
            gWindow.reset();
        }
<<<<<<< HEAD
        
        
=======
        TTF_Quit();
>>>>>>> origin/Editor

        // Quit SDL subsystems en orden inverso a su inicialización
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();

    } catch (const std::exception& e) {
        std::cerr << "Error during game shutdown: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error during game shutdown." << std::endl;
    }
    std::cout << "Game joinneado." << std::endl;
}



Game::~Game() {
    try {
        stop();
    } catch (const std::exception& e) {
        std::cerr << "Error in Game destructor: " << e.what() << std::endl;
    }
    printf("Game destroyed.\n");
}
