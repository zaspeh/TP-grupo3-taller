#ifndef DUCK_H
#define DUCK_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <unordered_map>
#include "ltexture.h"
#include "../common_src/game_state.h"
#include "animation.h"
#include "../common_src/utils.h"
#include "weapon.h"
#include "camera.h"
#include "armor.h"
#include <memory>

const int SPRITE_FLAP_WIDTH = 16;
const int SPRITE_FLAP_HEIGHT = 16;
const int SPRITE_WIDTH = 32;
const int SPRITE_HEIGHT = 32;
const int WALKING_ANIMATION_FRAMES = 6;
const int JUMPING_ANIMATION_FRAMES = 6;
const int DUCKING_ANIMATION_FRAMES = 5;
const int FLAPPING_ANIMATION_FRAMES = 6;
const float DUCK_SPEED = 1.0f;

class Duck
{
public:
    Duck(duck_t duckState, int screenWidth, int screenHeight, SDL_Renderer* renderer);
    void render(const Camera& camera, float zoom);
    bool loadTexture();
    void updateState(const duck_t& newDuckState);
    int getId() const { return duckState.id; }
    int getPosX() const { return duckState.pos.x; }
    int getPosY() const { return duckState.pos.y; }
    bool isAlive() const { return duckState.isAlive; }

private:
    duck_t duckState;
    bool isMoving;
    int color;
    std::unordered_map<DuckAnimationType, std::unique_ptr<Animation>> animations;
    SDL_Renderer* gRenderer;
    SDL_Rect scaleRect;
    int screenWidth;
    int screenHeight;
    DuckAnimationType currentAnimation;
    std::unique_ptr<Animation> wings;
    std::unique_ptr<Weapon> weapon;
    std::unique_ptr<Armor> armor;
};

#endif
