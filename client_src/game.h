#ifndef GAME_H
#define GAME_H

#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <memory>
#include <vector>
#include "ltexture.h"
#include "duck.h"
#include "../common_src/thread.h"
#include "../common_src/game_state.h"
#include "../common_src/queue.h"
#include "../common_src/utils.h"

constexpr float FRAME_DURATION_MS = 16.67f;

class Game : public Thread
{
    private:
        std::shared_ptr<Queue<game_state_t>> gameStateQueue;
        std::shared_ptr<Queue<uint8_t>> commandQueue;
        std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> gWindow;
        std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> gRenderer;
        std::vector<std::unique_ptr<Duck>> ducks;
        game_state_t gameState;
        void render();
        bool processEvents();
        void sendCommand(const uint8_t command);
        void update(game_state_t game_state);

    public:
        Game(std::shared_ptr<Queue<game_state_t>> gameStateQueue, std::shared_ptr<Queue<uint8_t>> commandQueue);
        ~Game();

        bool init();
        bool loadMedia();
        void run() override;
        void stop() override;
};

#endif // GAME_H