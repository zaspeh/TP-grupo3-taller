#include "duck.h"
#include <iostream>

Duck::Duck(duck_t duckState, int screenWidth, int screenHeight, SDL_Renderer* renderer)
    : duckState(duckState), gRenderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight), currentAnimation(WALKING)
{
    isMoving = false;
    scaleRect.w = SPRITE_WIDTH * 2;
    scaleRect.h = SPRITE_HEIGHT * 2;

    // Crear animaciones y verificar que estén inicializadas
    try {
        animations.emplace(WALKING, std::make_unique<Animation>(WALKING_ANIMATION_FRAMES, SPRITE_WIDTH, SPRITE_HEIGHT, renderer, WALKING));
        animations.emplace(JUMPING, std::make_unique<Animation>(JUMPING_ANIMATION_FRAMES, SPRITE_WIDTH, SPRITE_HEIGHT, renderer, JUMPING));
        animations.emplace(DUCKING, std::make_unique<Animation>(DUCKING_ANIMATION_FRAMES, SPRITE_WIDTH, SPRITE_HEIGHT, renderer, DUCKING));
    } catch (const std::bad_alloc& e) {
        std::cerr << "Failed to create animations: " << e.what() << std::endl;
    }
}

bool Duck::loadTexture() {
    bool allLoaded = true;

    if (!animations[WALKING]->loadTexture("client_src/duckyellow.png")) {
        std::cerr << "Failed to load WALKING texture." << std::endl;
        allLoaded = false;
    }
    if (!animations[JUMPING]->loadTexture("client_src/duckyellow.png")) {
        std::cerr << "Failed to load JUMPING texture." << std::endl;
        allLoaded = false;
    }
    if (!animations[DUCKING]->loadTexture("client_src/duckyellow.png")) {
        std::cerr << "Failed to load DUCKING texture." << std::endl;
        allLoaded = false;
    }

    return allLoaded;
}

void Duck::render() {
    bool faceLeft = (duckState.faceLeft == 1);

    if (animations[currentAnimation]) {
        animations[currentAnimation]->renderAnimation(duckState.pos.x, duckState.pos.y, scaleRect, faceLeft, isMoving);
    } else {
        std::cerr << "Attempted to render an uninitialized animation." << std::endl;
    }
}

void Duck::updateState(const duck_t& newDuckState) {
    if (duckState.pos.x != newDuckState.pos.x || 
    duckState.pos.y != newDuckState.pos.y ) {
        isMoving = true;
    } else {
        isMoving = false;
    }

    duckState = newDuckState;

    if (duckState.isJumping) {
        currentAnimation = JUMPING;
    } else if (duckState.isDucking) {
        currentAnimation = DUCKING;
    } else if (duckState.isFlaping) {
        currentAnimation = JUMPING;
    } else {
        currentAnimation = WALKING;
    }
}