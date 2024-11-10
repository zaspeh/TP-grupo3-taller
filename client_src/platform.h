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
    position_t pos;
public:
    Platform(position_t pos, SDL_Renderer* renderer);
    void render();
    bool loadTexture();
    void updateState(const position_t& newposState);
};

#endif