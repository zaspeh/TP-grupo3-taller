#include "game.h"

Game::Game(std::shared_ptr<Queue<game_state_t>> gameStateQueue, std::shared_ptr<Queue<uint8_t>> commandQueue)
    : gameStateQueue(gameStateQueue),
      commandQueue(commandQueue),
      gWindow(nullptr, SDL_DestroyWindow),
      gRenderer(nullptr, SDL_DestroyRenderer),
      duck(nullptr)
{
}


Game::~Game()
{
    stop();
}

bool Game::loadMedia()
{
    return duck->loadTexture();
}

bool Game::processEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            return true;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym) 
            {
                case SDLK_UP:
                    sendCommand(JUMP);
                    break;
                case SDLK_DOWN:
                    sendCommand(FLOOR);
                    break;
                case SDLK_LEFT:
                    sendCommand(MOVE_LEFT);
                    break;
                case SDLK_RIGHT:
                    sendCommand(MOVE_RIGHT);
                    break;
                default:
                    break;
            }
        }
    }
    return false;
}

void Game::sendCommand(const uint8_t command){
    // Logica para cargar la cola de comandos
    commandQueue->try_push(command);
}

void Game::run()
{
    init();
    //loadMedia();
    bool quit = false;
    Uint32 frameDelay = 10;

    while (!quit)
    {
        Uint32 startTime = SDL_GetTicks();

        quit = processEvents();

        SDL_SetRenderDrawColor(gRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer.get());
        game_state_t game_state; 
        gameStateQueue->try_pop(game_state);
        if(game_state.level.ducks[0].pos.x != 0)
            
        //std::cout << "nueva posicion: " << static_cast<int>(game_state.level.ducks[0].pos.x) << ", " << static_cast<int>(game_state.level.ducks[0].pos.y) << std::endl;

        update(game_state);
        render();

        SDL_RenderPresent(gRenderer.get());

        Uint32 frameTime = SDL_GetTicks() - startTime;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    stop(); //Cuando termina este loop se cierra la ventana, entonces tenemos que hacer que se cierren los demas hilos
}

void Game::render(){
   duck->render();
}

void Game::update(game_state_t game_state){
    for (int i = 0; i < game_state.level.num_ducks; i++){
        duck->updateState(game_state.level.ducks[i]); // actualiza solo 1 pato xd 
    }
}

void Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        // Puedes lanzar una excepción aquí si es necesario
    }

    gWindow.reset(SDL_CreateWindow("Duck", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
    if (!gWindow)
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        // Puedes lanzar una excepción aquí si es necesario
    }

    gRenderer.reset(SDL_CreateRenderer(gWindow.get(), -1, SDL_RENDERER_ACCELERATED));
    if (!gRenderer)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        // Puedes lanzar una excepción aquí si es necesario
    }
    SDL_SetRenderDrawColor(gRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        // Puedes lanzar una excepción aquí si es necesario
    }

    // Crear la instancia de Duck
    duck = std::make_unique<Duck>(gameState.level.ducks[0], SCREEN_WIDTH, SCREEN_HEIGHT, gRenderer.get());
    loadMedia();
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
