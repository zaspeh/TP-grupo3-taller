#include "game.h"

Game::Game(std::shared_ptr<Queue<game_state_t>> gameStateQueue, std::shared_ptr<Queue<uint8_t>> commandQueue)
    : gameStateQueue(gameStateQueue),
      commandQueue(commandQueue),
      gWindow(nullptr, SDL_DestroyWindow),
      gRenderer(nullptr, SDL_DestroyRenderer)
{
}

Game::~Game()
{
    stop();
}

bool Game::loadMedia()
{
    if(!ducks.empty()){
        
        return ducks[0]->loadTexture();
    }
    return false;
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
    // Logica para cargar la cola de comandos
    commandQueue->try_push(command);
}

void Game::run()
{
    if (!init() || !loadMedia()) {
        printf("Failed to initialize game or load media.\n");
        return;
    }

    bool quit = false;
    auto next_frame = std::chrono::steady_clock::now();

    while (!quit) {
        quit = processEvents();

        SDL_SetRenderDrawColor(gRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer.get());

        game_state_t game_state;
        // Intentamos obtener el último estado disponible, sin necesidad de vaciar la cola
        if (gameStateQueue->try_pop(game_state)) {  
            update(game_state);
        }

        render();

        SDL_RenderPresent(gRenderer.get());

        // Control de tiempo para limitar el FPS
        next_frame += std::chrono::milliseconds(static_cast<int>(FRAME_DURATION_MS));
        std::this_thread::sleep_until(next_frame);

        // Ajuste de tiempo para evitar desfasajes
        auto frame_end = std::chrono::steady_clock::now();
        if (frame_end > next_frame) {
            next_frame = frame_end;
        }
    }

    stop();
}

void Game::render() {
    for (auto& duck: ducks) {
        duck->render();
    }
}

void Game::update(game_state_t game_state) {
    for (int i = 0; i < game_state.level.num_ducks; i++) {
        ducks[i]->updateState(game_state.level.ducks[i]);
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

    ducks.emplace_back(std::make_unique<Duck>(gameState.level.ducks[0], SCREEN_WIDTH, SCREEN_HEIGHT, gRenderer.get())); // inicializo en el pato 0

    //duck = std::make_unique<Duck>(gameState.level.ducks[0], SCREEN_WIDTH, SCREEN_HEIGHT, gRenderer.get());
    return true;
}

void Game::stop()
{
    //delete duck;

    //SDL_DestroyRenderer(gRenderer);
    //SDL_DestroyWindow(gWindow);
    //gWindow = NULL;
    //gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();
}
