#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "game.h"

int main(int argc, char* args[])
{
    Game game;

    if (!game.init())
    {
        printf("Failed to initialize!\n");
        return -1;
    }

    if (!game.loadMedia())
    {
        printf("Failed to load media!\n");
        return -1;
    }

    game.run();

    return 0;
}
