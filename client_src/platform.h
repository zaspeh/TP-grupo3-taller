#ifndef PLATFORM_H
#define PLATFORM_H

#include "../common_src/game_state.h"
#include "ltexture.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>

class Platform{
private:
    std::unique_ptr<LTexture> platformTexture;
    platform_t platform;
public:
    Platform(platform_t platform_t, SDL_Renderer* renderer);
    void render();
    bool loadTexture();
    void updateState(const platform_t& newPlatformState);
};

#endif