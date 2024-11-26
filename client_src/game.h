#ifndef GAME_H
#define GAME_H

#include <mutex>
#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <memory>
#include <vector>
#include <SDL2/SDL_ttf.h>
#include "music.h" // musiquita bien chill de cojones
#include "ltexture.h"
#include "duck.h"
#include "platform.h"
#include "client.h"
#include "spawn_place.h"
#include "projectile.h"
#include "box.h"
#include "armor.h"
#include "zoom.h"
#include "camera.h"
#include "banana.h"
#include "calculatorManager.h"
#include "../common_src/thread.h"
#include "../common_src/game_state.h"
#include "../common_src/queue.h"
#include "../common_src/utils.h"

constexpr float FRAME_DURATION_MS = 16.67f;

class Client;

class Game : public Thread
{
    private:
        Music music;
        Mix_Music* backgroundMusic;
        std::vector<std::unique_ptr<Banana>> bananas;
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
        std::vector<std::unique_ptr<Animation>> explotions;
        std::unique_ptr<LTexture> background;
        game_state_t gameState;
        std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> gFont{nullptr, TTF_CloseFont};
        Camera camera;
        Zoom zoom;
        void render();
        bool processEvents();
        void sendCommand(const uint8_t command);
        void update(game_state_t game_state);
        void renderText(const std::string& message, int x, int y, int color);
        std::mutex sdl_mutex;
        Client& client;

    public:
        Game(std::shared_ptr<Queue<game_state_t>> gameStateQueue, std::shared_ptr<Queue<uint8_t>> commandQueue, Client& client);
        ~Game();
        void loadMusic();
        void playMusic();
        void stopMusic();
        void cleanupMusic();
        bool init();
        void initializeGameObjects();
        bool loadMedia();
        void run() override;
        void stop() override;
};

#endif // GAME_H