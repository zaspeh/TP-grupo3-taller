#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <memory>
#include "ltexture.h"
#include "duck.h"
#include "../common_src/thread.h"
#include "../common_src/game_state.h"
#include "../common_src/queue.h"
#include "../common_src/utils.h"

class Game : public Thread
{
    private:
        std::shared_ptr<Queue<game_state_t>> gameStateQueue;
        std::shared_ptr<Queue<uint8_t>> commandQueue;
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> gWindow;
        std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> gRenderer;
        std::unique_ptr<Duck> duck;
        game_state_t gameState;
        void render();
        bool processEvents();
        void sendCommand(const uint8_t command);
        void update(game_state_t game_state);

    public:
        Game(std::shared_ptr<Queue<game_state_t>> gameState, std::shared_ptr<Queue<uint8_t>> commandQueue);
        ~Game();

        void init();
        bool loadMedia();
        void run() override;
        //void close();
        void stop() override;
        
};

#endif
