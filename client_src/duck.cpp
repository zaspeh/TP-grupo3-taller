#include "duck.h"
#include <iostream>

Duck::Duck(duck_t duckState, int screenWidth, int screenHeight, SDL_Renderer* renderer)
    : duckState(duckState), gRenderer(renderer), screenWidth(screenWidth), screenHeight(screenHeight), currentAnimation(WALKING)
{
    isMoving = false;
    scaleRect.w = SPRITE_WIDTH * 2;
    scaleRect.h = SPRITE_HEIGHT * 2;

    try {
        animations.emplace(WALKING, std::make_unique<Animation>(WALKING_ANIMATION_FRAMES, SPRITE_WIDTH, SPRITE_HEIGHT, renderer, WALKING));
        animations.emplace(JUMPING, std::make_unique<Animation>(JUMPING_ANIMATION_FRAMES, SPRITE_WIDTH, SPRITE_HEIGHT, renderer, JUMPING));
        animations.emplace(DUCKING, std::make_unique<Animation>(DUCKING_ANIMATION_FRAMES, SPRITE_WIDTH, SPRITE_HEIGHT, renderer, DUCKING));
    } catch (const std::bad_alloc& e) {
        std::cerr << "Failed to create animations: " << e.what() << std::endl;
    }

    wings = std::make_unique<Animation>(FLAPPING_ANIMATION_FRAMES, SPRITE_FLAP_WIDTH, SPRITE_FLAP_HEIGHT, renderer, FLAPPING);
}

bool Duck::loadTexture() {
    bool allLoaded = true;
    std::string path = "client_src/duckyellow.png";

    if(duckState.id == 0){
        path = "client_src/duckyellow.png";
    }else if(duckState.id == 1){
        path = "client_src/duckgray.png";
    }else if(duckState.id == 2){
        path = "client_src/duckorange.png";
    }else{
        path = "client_src/duckwhite.png";
    }

    if (!animations[WALKING]->loadTexture(path)) {
        std::cerr << "Failed to load WALKING texture." << std::endl;
        allLoaded = false;
    }

    if (!animations[JUMPING]->loadTexture(path)) {
        std::cerr << "Failed to load JUMPING texture." << std::endl;
        allLoaded = false;
    }

    if (!animations[DUCKING]->loadTexture(path)) {
        std::cerr << "Failed to load DUCKING texture." << std::endl;
        allLoaded = false;
    }

    if(!wings->loadTexture("client_src/duckyellowflap.png")){
        std::cerr << "Failed to load DUCKING texture." << std::endl;
        allLoaded = false;
    }
    
    return allLoaded;
}

void Duck::render() {
    bool faceLeft = (duckState.faceLeft == 1);

    if (animations[currentAnimation]) {
        animations[currentAnimation]->renderAnimation(duckState.pos.x, duckState.pos.y, scaleRect, faceLeft, isMoving);
        SDL_Rect scaleFlap;
        scaleFlap.h = SPRITE_FLAP_HEIGHT*2;
        scaleFlap.w = SPRITE_FLAP_WIDTH*2;
        float posY = duckState.pos.y + 25;
        float posX = duckState.pos.x + 10;
        if (faceLeft){
            posX = posX + 10;
        }
        wings->renderAnimation(posX, posY, scaleFlap, faceLeft, true);
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