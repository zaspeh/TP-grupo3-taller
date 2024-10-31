#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "ltexture.h"
#include "duck.h"

class Game
{
public:
    Game();
    ~Game();

    bool init();
    bool loadMedia();
    void run();
    void close();

private:
    GameState gameState;
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    Duck* duck;
    void render();
    void update(GameState game_state);
};

#endif
