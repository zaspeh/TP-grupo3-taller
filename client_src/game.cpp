#include "game.h"
#include <algorithm>  // Añadir este include al principio del archivo

Game::Game(std::shared_ptr<Queue<game_state_t>> gameStateQueue, std::shared_ptr<Queue<uint8_t>> commandQueue)
    : gameStateQueue(gameStateQueue),
      commandQueue(commandQueue),
      gWindow(nullptr, SDL_DestroyWindow),
      gRenderer(nullptr, SDL_DestroyRenderer)
{
    gameState = gameStateQueue->pop();
    ducks.reserve(MAX_DUCKS);
    platforms.reserve(MAX_PLATFORMS);
    spawns.reserve(MAX_SPAWN_PLACES);
    droppedWeapons.reserve(MAX_ITEMS);
    droppedArmors.reserve(MAX_ITEMS);
    projectiles.reserve(MAX_PROJECTILES);
    boxes.reserve(MAX_BOXES);
}

Game::~Game()
{
    stop();
    printf("Game destroyed.\n");
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

bool Game::loadMedia() {
    bool success = true;
    
    try {
        // Load background
        background = std::make_unique<LTexture>(gRenderer.get());
        if (!background || !background->loadFromFile("client_src/forest.png")) {
            throw std::runtime_error("Failed to load background texture");
        }

        // Initialize game objects with proper error handling
        initializeGameObjects();
        
    } catch (const std::exception& e) {
        std::cerr << "Error loading media: " << e.what() << std::endl;
        success = false;
    }
    
    return success;
}

void Game::initializeGameObjects() {
    // Initialize with proper error checking and exception handling
    if (gameState.level.num_ducks > MAX_DUCKS ||
        gameState.level.num_platforms > MAX_PLATFORMS ||
        gameState.level.num_spawn_places > MAX_SPAWN_PLACES ||
        gameState.level.num_boxes > MAX_BOXES) {
        throw std::runtime_error("Game state exceeds maximum allowed objects");
    }

    // Initialize vectors with proper sizes
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

// Variables para limitar la frecuencia de envío de comandos
const std::chrono::milliseconds COMMAND_INTERVAL(50); // Para movimiento
const std::chrono::milliseconds SHOOT_INTERVAL(200); // Para disparos, 200ms entre cada disparo
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
            return true;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP: sendCommand(JUMP); break;
                case SDLK_DOWN: sendCommand(FLOOR); break;
                case SDLK_LEFT: leftPressed = true; break;
                case SDLK_RIGHT: rightPressed = true; break;
                case SDLK_RSHIFT: sendCommand(TAKE_WEAPON); break;
                case SDLK_LCTRL: shootPressed = true; break;
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

    // Deberìa ser màs pausada la cantidad de veces que se envia el dispa

    // Control de la frecuencia de envío de comandos
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

void Game::run()
{
    if (!init() || !loadMedia()) {
        printf("Failed to initialize game or load media.\n");
        return;
    }

    //printf("Game initialized and media loaded successfully.\n");

    bool quit = false;
    auto next_frame = std::chrono::steady_clock::now();

    while (!quit) {
        quit = processEvents();

        SDL_SetRenderDrawColor(gRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer.get());


        while (gameStateQueue->try_pop(gameState)) {
            continue;
        }

        //printf("Game state updated.\n");
        //std::cout << "Posicion del pato: " << static_cast<int>(gameState.level.ducks[0].pos.x) << " " << static_cast<int>(gameState.level.ducks[0].pos.y) << std::endl; 
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

    stop();
    printf("Game loop ended.\n");
}

void Game::render() {
    if (!gRenderer) {
        std::cerr << "Renderer is null" << std::endl;
        return;
    }
    
    SDL_SetRenderDrawColor(gRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(gRenderer.get());

    // Render background
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
            projectile->render();
        }
    }

    SDL_RenderPresent(gRenderer.get());
}

void Game::update(game_state_t gameState) {
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

bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow.reset(SDL_CreateWindow("Duck", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
    if (!gWindow) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    gRenderer.reset(SDL_CreateRenderer(gWindow.get(), -1, SDL_RENDERER_ACCELERATED));
    if (!gRenderer) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderDrawColor(gRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    ducks.resize(gameState.level.num_ducks); 
    printf("Initialized ducks vector with %d ducks.\n", gameState.level.num_ducks);

    platforms.resize(gameState.level.num_platforms); 
    printf("Initialized ducks vector with %d platforms.\n", gameState.level.num_platforms);

    return true;
}


void Game::stop() {
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

        // Clear SDL-specific resources
        if (gRenderer) {
            gRenderer.reset();
            std::cout << "Renderer destroyed successfully." << std::endl;
        }
        
        if (gWindow) {
            gWindow.reset();
            std::cout << "Window destroyed successfully." << std::endl;
        }

        // Quit SDL subsystems
        IMG_Quit();
        std::cout << "SDL_image subsystem terminated." << std::endl;
        
        SDL_Quit();
        std::cout << "SDL subsystems terminated." << std::endl;

        std::cout << "Game stopped successfully." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error during game shutdown: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error during game shutdown." << std::endl;
    }
}


/*
Error: socket recv failedBad file descriptor
==22915== Thread 4:
==22915== Invalid read of size 8
==22915==    at 0x493F99F: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x12403C: Game::render() (game.cpp:288)
==22915==    by 0x12375E: Game::run() (game.cpp:225)
==22915==    by 0x1257FF: Thread::main() (thread.h:43)
==22915==    by 0x131C5B: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22915==    by 0x131BAE: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22915==    by 0x131B0E: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22915==    by 0x131AC3: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==22915==    by 0x131AA3: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==22915==    by 0x4B0F252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==22915==    by 0x4D15AC2: start_thread (pthread_create.c:442)
==22915==    by 0x4DA6A03: clone (clone.S:100)
==22915==  Address 0x154a6f60 is 0 bytes inside a block of size 240 free'd
==22915==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22915==    by 0x4945559: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x49456F0: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x1251CD: Game::stop() (game.cpp:453)
==22915==    by 0x116916: Client::stop() (client.cpp:91)
==22915==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22915==    by 0x116849: Client::run() (client.cpp:79)
==22915==    by 0x132071: main (main.cpp:11)
==22915==  Block was alloc'd at
==22915==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22915==    by 0x493D0C1: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x124FF3: Game::init() (game.cpp:420)
==22915==    by 0x123645: Game::run() (game.cpp:200)
==22915==    by 0x1257FF: Thread::main() (thread.h:43)
==22915==    by 0x131C5B: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22915==    by 0x131BAE: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22915==    by 0x131B0E: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22915==    by 0x131AC3: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==22915==    by 0x131AA3: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==22915==    by 0x4B0F252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==22915==    by 0x4D15AC2: start_thread (pthread_create.c:442)
==22915== 
==22915== Invalid read of size 8
==22915==    at 0x493F99F: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x123776: Game::run() (game.cpp:227)
==22915==    by 0x1257FF: Thread::main() (thread.h:43)
==22915==    by 0x131C5B: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22915==    by 0x131BAE: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22915==    by 0x131B0E: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22915==    by 0x131AC3: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==22915==    by 0x131AA3: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==22915==    by 0x4B0F252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==22915==    by 0x4D15AC2: start_thread (pthread_create.c:442)
==22915==    by 0x4DA6A03: clone (clone.S:100)
==22915==  Address 0x154a6f60 is 0 bytes inside a block of size 240 free'd
==22915==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22915==    by 0x4945559: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x49456F0: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x1251CD: Game::stop() (game.cpp:453)
==22915==    by 0x116916: Client::stop() (client.cpp:91)
==22915==    by 0x1162A4: Client::checkIfClose() (client.cpp:14)
==22915==    by 0x116849: Client::run() (client.cpp:79)
==22915==    by 0x132071: main (main.cpp:11)
==22915==  Block was alloc'd at
==22915==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22915==    by 0x493D0C1: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x124FF3: Game::init() (game.cpp:420)
==22915==    by 0x123645: Game::run() (game.cpp:200)
==22915==    by 0x1257FF: Thread::main() (thread.h:43)
==22915==    by 0x131C5B: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==22915==    by 0x131BAE: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==22915==    by 0x131B0E: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==22915==    by 0x131AC3: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==22915==    by 0x131AA3: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==22915==    by 0x4B0F252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==22915==    by 0x4D15AC2: start_thread (pthread_create.c:442)
==22915== 
==22915== Invalid read of size 8
==22915==    at 0x94EA891: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8C3DE62: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8C01C60: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x48D7022: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x48D2D83: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x131E58: LTexture::free() (ltexture.cpp:36)
==22915==    by 0x131D44: LTexture::loadFromFile(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (ltexture.cpp:13)
==22915==    by 0x115F9D: Box::loadTexture() (box.cpp:43)
==22915==    by 0x116165: Box::updateState(box_t const&) (box.cpp:62)
==22915==    by 0x124E99: Game::update(game_state_t) (game.cpp:404)
==22915==    by 0x12374B: Game::run() (game.cpp:223)
==22915==    by 0x1257FF: Thread::main() (thread.h:43)
==22915==  Address 0x895c140 is 576 bytes inside a block of size 86,896 free'd
==22915==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22915==    by 0x8B6FD9B: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x85EF1DA: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x85E0F21: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x85E106C: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x510A891: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22915==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x1251CD: Game::stop() (game.cpp:453)
==22915==    by 0x116916: Client::stop() (client.cpp:91)
==22915==  Block was alloc'd at
==22915==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22915==    by 0x94DB8D3: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8B66937: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x91A7EA6: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8B687D3: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8B70748: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x85EF762: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x85E1330: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x85DCD77: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x4967B2C: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x4967F0C: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x493EE3B: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915== 
==22915== Invalid read of size 1
==22915==    at 0x94EA898: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8C3DE62: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8C01C60: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x48D7022: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x48D2D83: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x131E58: LTexture::free() (ltexture.cpp:36)
==22915==    by 0x131D44: LTexture::loadFromFile(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (ltexture.cpp:13)
==22915==    by 0x115F9D: Box::loadTexture() (box.cpp:43)
==22915==    by 0x116165: Box::updateState(box_t const&) (box.cpp:62)
==22915==    by 0x124E99: Game::update(game_state_t) (game.cpp:404)
==22915==    by 0x12374B: Game::run() (game.cpp:223)
==22915==    by 0x1257FF: Thread::main() (thread.h:43)
==22915==  Address 0x895a150 is 256 bytes inside a block of size 544 free'd
==22915==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22915==    by 0x94DB670: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8B6FD9B: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x85EF1DA: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x85E0F21: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x85E106C: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x510A891: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22915==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x1251CD: Game::stop() (game.cpp:453)
==22915==  Block was alloc'd at
==22915==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22915==    by 0x952093E: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x951E6AE: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8B6692A: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x91A7EA6: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8B687D3: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8B70748: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x85EF762: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x85E1330: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x85DCD77: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x4967B2C: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x4967F0C: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915== 
==22915== Invalid read of size 8
==22915==    at 0x8C1A3A6: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8BEEDEC: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8C0193F: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8C01A5C: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8C01C6C: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x48D7022: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x48D2D83: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x131E58: LTexture::free() (ltexture.cpp:36)
==22915==    by 0x131D44: LTexture::loadFromFile(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (ltexture.cpp:13)
==22915==    by 0x115F9D: Box::loadTexture() (box.cpp:43)
==22915==    by 0x116165: Box::updateState(box_t const&) (box.cpp:62)
==22915==    by 0x124E99: Game::update(game_state_t) (game.cpp:404)
==22915==  Address 0x895bff0 is 240 bytes inside a block of size 86,896 free'd
==22915==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22915==    by 0x8B6FD9B: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x85EF1DA: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x85E0F21: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x85E106C: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x510A891: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22915==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x1251CD: Game::stop() (game.cpp:453)
==22915==    by 0x116916: Client::stop() (client.cpp:91)
==22915==  Block was alloc'd at
==22915==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22915==    by 0x94DB8D3: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8B66937: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x91A7EA6: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8B687D3: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8B70748: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x85EF762: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x85E1330: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x85DCD77: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x4967B2C: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x4967F0C: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x493EE3B: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915== 
==22915== Invalid read of size 4
==22915==    at 0x94ED2C0: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8C1A3AB: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8BEEDEC: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8C0193F: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8C01A5C: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8C01C6C: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x48D7022: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x48D2D83: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x131E58: LTexture::free() (ltexture.cpp:36)
==22915==    by 0x131D44: LTexture::loadFromFile(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >) (ltexture.cpp:13)
==22915==    by 0x115F9D: Box::loadTexture() (box.cpp:43)
==22915==    by 0x116165: Box::updateState(box_t const&) (box.cpp:62)
==22915==  Address 0x895c184 is 644 bytes inside a block of size 86,896 free'd
==22915==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22915==    by 0x8B6FD9B: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x85EF1DA: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x85E0F21: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x85E106C: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x510A891: XCloseDisplay (in /usr/lib/x86_64-linux-gnu/libX11.so.6.4.0)
==22915==    by 0x496A53D: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x494583A: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x489C1E3: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x489C4B5: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x1251CD: Game::stop() (game.cpp:453)
==22915==    by 0x116916: Client::stop() (client.cpp:91)
==22915==  Block was alloc'd at
==22915==    at 0x484DA83: calloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==22915==    by 0x94DB8D3: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8B66937: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x91A7EA6: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8B687D3: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x8B70748: ??? (in /usr/lib/x86_64-linux-gnu/dri/vmwgfx_dri.so)
==22915==    by 0x85EF762: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x85E1330: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x85DCD77: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_mesa.so.0.0.0)
==22915==    by 0x4967B2C: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x4967F0C: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915==    by 0x493EE3B: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.18.2)
==22915== 

*/