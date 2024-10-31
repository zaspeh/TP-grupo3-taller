#include "game.h"

Game::Game() : gWindow(NULL), gRenderer(NULL), duck(NULL) {}

Game::~Game()
{
    close();
}

bool Game::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow("Duck", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == NULL)
    {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == NULL)
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    duck = new Duck(gameState.levels[gameState.current_level].ducks[0], SCREEN_WIDTH, SCREEN_HEIGHT, gRenderer);

    return true;
}

bool Game::loadMedia()
{
    return duck->loadTexture();
}

void Game::run()
{
    bool quit = false;
    SDL_Event e;
    Uint32 frameDelay = 10;

    while (!quit)
    {
        Uint32 startTime = SDL_GetTicks();

        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

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
