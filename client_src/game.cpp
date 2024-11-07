#include "game.h"

Game::Game(std::shared_ptr<Queue<game_state_t>> gameStateQueue, std::shared_ptr<Queue<uint8_t>> commandQueue)
    : gameStateQueue(gameStateQueue),
      commandQueue(commandQueue),
      gWindow(nullptr, SDL_DestroyWindow),
      gRenderer(nullptr, SDL_DestroyRenderer)
{
    gameState = gameStateQueue->pop();
    printf("Game initialized with game state.\n");
}

Game::~Game()
{
    stop();
    printf("Game destroyed.\n");
}

bool Game::loadMedia()
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
    return charged;
}

#include <chrono>

// Variables para limitar la frecuencia de envío de comandos
const std::chrono::milliseconds COMMAND_INTERVAL(50); // Intervalo mínimo de 50 ms
std::chrono::steady_clock::time_point lastCommandTime = std::chrono::steady_clock::now();

bool leftPressed = false;
bool rightPressed = false;

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
                default: break;
            }
        } else if (e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym) {
                case SDLK_LEFT: leftPressed = false; break;
                case SDLK_RIGHT: rightPressed = false; break;
                default: break;
            }
        }
    }

    // Control de la frecuencia de envío de comandos
    auto currentTime = std::chrono::steady_clock::now();
    if (currentTime - lastCommandTime >= COMMAND_INTERVAL) {
        if (leftPressed) sendCommand(MOVE_LEFT);
        if (rightPressed) sendCommand(MOVE_RIGHT);
        lastCommandTime = currentTime; // Actualizar el último envío
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

    printf("Game initialized and media loaded successfully.\n");

    bool quit = false;
    auto next_frame = std::chrono::steady_clock::now();

    while (!quit) {
        quit = processEvents();

        SDL_SetRenderDrawColor(gRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer.get());


        while (gameStateQueue->try_pop(gameState)) {
            continue;
        }

        printf("Game state updated.\n");
        std::cout << "Posicion del pato: " << static_cast<int>(gameState.level.ducks[0].pos.x) << " " << static_cast<int>(gameState.level.ducks[0].pos.y) << std::endl; 
        update(gameState);

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
    for (size_t i = 0; i < ducks.size(); i++) {
        if (ducks[i]) {
            ducks[i]->render();
        }
    }
}

void Game::update(game_state_t gameState) {
    // Ajustar el tamaño del vector 'ducks' para que coincida con el número total de patos en el 'gameState'
    if (ducks.size() < gameState.level.num_ducks) {
        ducks.resize(gameState.level.num_ducks);
        for (int i = static_cast<int>(ducks.size()) - static_cast<int>(gameState.level.num_ducks); i < static_cast<int>(ducks.size()); i++) {
            ducks[i] = std::make_unique<Duck>(gameState.level.ducks[i], SCREEN_WIDTH, SCREEN_HEIGHT, gRenderer.get());
            ducks[i]->loadTexture();
            std::cout << "New duck initialized at index " << i << std::endl;
        }
    }

    // Si hay menos patos en el 'gameState', eliminar los que sobran (en caso de desconexión de patos)
    while (ducks.size() > gameState.level.num_ducks) { // esto creo que no nos interesa
        ducks.pop_back();
    }

    // Actualizar el estado de cada pato en el vector
    for (int i = 0; i < gameState.level.num_ducks; i++) {
        ducks[i]->updateState(gameState.level.ducks[i]);
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

    ducks.resize(gameState.level.num_ducks);  // Inicializa el vector con el tamaño correcto
    printf("Initialized ducks vector with %d ducks.\n", gameState.level.num_ducks);

    return true;
}


void Game::stop()
{
    IMG_Quit();
    SDL_Quit();
    printf("Game stopped.\n");
}