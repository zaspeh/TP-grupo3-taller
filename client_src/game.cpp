#include "game.h"

Game::Game(std::make_shared<Queue<GameState>>  gameState, std::shared_ptr<Queue<uint8_t>> commandQueue) : gameStateQueue(gameState), commandQueue(commandQueue) ,gWindow(NULL), gRenderer(NULL), duck(NULL) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        //CATCH EXP
    }

    gWindow = SDL_CreateWindow("Duck", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        //CATCH EXP
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        //CATCH EXP
    }
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        //CATCH EXP
    }

    duck = new Duck(gameState.levels[gameState.current_level].ducks[0], SCREEN_WIDTH, SCREEN_HEIGHT, gRenderer);
}

Game::~Game()
{
    close();
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

            /*
            #define MOVE_LEFT 0x01
            #define MOVE_RIGHT 0x02
            #define JUMP 0x03
            #define TAKE_WEAPON 0x04
            #define SHOOT 0x05
            #define LOOK_UP 0x06
            #define FLOOR 0x07
            */
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
    commandQueue.try_push(command);
}

void Game::run()
{
    bool quit = false;
    Uint32 frameDelay = 10;

    while (!quit)
    {
        Uint32 startTime = SDL_GetTicks();

        quit = processEvents();

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        //GameState game_state; //Esto lo tengo que recibir por server
        //update(game_state);
        render();

        SDL_RenderPresent(gRenderer);

        Uint32 frameTime = SDL_GetTicks() - startTime;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void Game::render(){
   duck->render();
}

void Game::update(GameState game_state){
    duck->updateState(game_state.levels[game_state.current_level].ducks[0]);
}

void Game::close()
{
    delete duck;

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();
}
