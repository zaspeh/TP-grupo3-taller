#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "game.h"
#include "client.h"

int main(int argc, char* args[])
{
    Client client(args[1], args[2]);
    client.run();

    return 0;
}
