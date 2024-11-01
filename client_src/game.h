#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "ltexture.h"
#include "duck.h"
#include "./common/unix.h"

class Game : public Thread
{
    private:
        GameState gameState;
        SDL_Window* gWindow;
        SDL_Renderer* gRenderer;
        Duck* duck;
        void render();
        bool processEvents();
        void sendCommand(const uint8_t command);
        void update(GameState game_state);
        std::make_shared<Queue<GameState>> gameStateQueue;
        std::make_shared<Queue<uint8_t>> commandQueue;

    public:
        Game();
        ~Game();

        bool init();
        bool loadMedia();
        void run();
        void close();
        
};

#endif
