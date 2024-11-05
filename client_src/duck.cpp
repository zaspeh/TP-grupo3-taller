#include "duck.h"

Duck::Duck(duck_t duckState, int screenWidth, int screenHeight,  SDL_Renderer* renderer)
    : duckState(duckState), gRenderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight), currentAnimation(WALKING)
{
    scaleRect.w = SPRITE_WIDTH * 2;
    scaleRect.h = SPRITE_HEIGHT * 2;
    animations.emplace(WALKING, std::make_unique<Animation>(WALKING_ANIMATION_FRAMES, SPRITE_WIDTH, SPRITE_HEIGHT, renderer, WALKING));
    animations.emplace(JUMPING, std::make_unique<Animation>(JUMPING_ANIMATION_FRAMES, SPRITE_WIDTH, SPRITE_HEIGHT, renderer, JUMPING));
    animations.emplace(DUCKING, std::make_unique<Animation>(DUCKING_ANIMATION_FRAMES, SPRITE_WIDTH, SPRITE_HEIGHT, renderer, DUCKING));
}

bool Duck::loadTexture(){
    bool allLoaded = true;
    if (!animations[WALKING]->loadTexture("client_src/duckyellow.png")) {
        allLoaded = false;
    }
    if (!animations[JUMPING]->loadTexture("client_src/duckyellow.png")) {
        allLoaded = false;
    }
    if (!animations[DUCKING]->loadTexture("client_src/duckyellow.png")) {
        allLoaded = false;
    }
    return allLoaded;
}

void Duck::render()
{
    bool faceLeft = (duckState.faceLeft == 1);
    animations[currentAnimation]->renderAnimation(duckState.pos.x, duckState.pos.y, scaleRect, faceLeft);
}

void Duck::updateState(const duck_t& newDuckState)
{
    duckState = newDuckState;
    if (duckState.isJumping) {
        currentAnimation = JUMPING;
    } else if (duckState.isDucking){
        currentAnimation = DUCKING;
    }
    else {
        currentAnimation = WALKING;
    }
}
