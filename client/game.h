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
    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;
    Duck* duck;
};

#endif