#ifndef GAME_H
#define GAME_H

#include <mutex>
#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <memory>
#include <vector>
#include "ltexture.h"
#include "duck.h"
#include "platform.h"
#include "spawn_place.h"
#include "projectile.h"
#include "box.h"
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
        std::vector<std::unique_ptr<Platform>> platforms;
        std::vector<std::unique_ptr<SpawnPlace>> spawns;
        std::vector<std::unique_ptr<Weapon>> droppedWeapons;
        std::vector<std::unique_ptr<Armor>> droppedArmors;
        std::vector<std::unique_ptr<Projectile>> projectiles;
        std::vector<std::unique_ptr<Box>> boxes;
        std::unique_ptr<LTexture> background;
        game_state_t gameState;
        void render();
        bool processEvents();
        void sendCommand(const uint8_t command);
        void update(game_state_t game_state);
        std::mutex sdl_mutex;

    public:
        Game(std::shared_ptr<Queue<game_state_t>> gameStateQueue, std::shared_ptr<Queue<uint8_t>> commandQueue);
        ~Game();

        bool init();
        void initializeGameObjects();
        bool loadMedia();
        void run() override;
        void stop() override;
};

#endif // GAME_H