#ifndef DUCK_H
#define DUCK_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <unordered_map>
#include "ltexture.h"
#include "../common_src/game_state.h"
#include "animation.h"
#include <memory>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SPRITE_WIDTH = 32;
const int SPRITE_HEIGHT = 32;
const int WALKING_ANIMATION_FRAMES = 6;
const int JUMPING_ANIMATION_FRAMES = 6;
const int DUCKING_ANIMATION_FRAMES = 5;
const float DUCK_SPEED = 1.0f;

class Duck
{
public:
    Duck(duck_t duckState, int screenWidth, int screenHeight, SDL_Renderer* renderer);
    void render();
    bool loadTexture();
    void updateState(const duck_t& newDuckState);

private:
    duck_t duckState;
    std::unordered_map<DuckAnimationType, std::unique_ptr<Animation>> animations;
    SDL_Renderer* gRenderer;
    SDL_Rect scaleRect;
    int screenWidth;
    int screenHeight;
    DuckAnimationType currentAnimation;
};

#endif
