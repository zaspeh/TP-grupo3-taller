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
        armorAnimations.emplace(WALKING, std::make_unique<Animation>(WALKING_ANIMATION_FRAMES-1, SPRITE_WIDTH, SPRITE_HEIGHT, renderer, ARMOR));
        armorAnimations.emplace(JUMPING, std::make_unique<Animation>(JUMPING_ANIMATION_FRAMES, SPRITE_WIDTH, SPRITE_HEIGHT, renderer, JUMPING));
        armorAnimations.emplace(DUCKING, std::make_unique<Animation>(DUCKING_ANIMATION_FRAMES, SPRITE_WIDTH, SPRITE_HEIGHT, renderer, DUCKING));
        wings = std::make_unique<Animation>(FLAPPING_ANIMATION_FRAMES, SPRITE_FLAP_WIDTH, SPRITE_FLAP_HEIGHT, renderer, FLAPPING);
        
        weapon = std::make_unique<Weapon>(duckState.equipped_weapon, renderer);
        armor = std::make_unique<Armor>(duckState.chestplate, renderer);

    } catch (const std::bad_alloc& e) {
        std::cerr << "Failed to create animations or equipment: " << e.what() << std::endl;
    }
}

bool Duck::loadTexture() {
    bool allLoaded = true;
    std::string pathDuck = "client_src/duckyellow.png";
    std::string pathWings = "client_src/duckyellowflap.png";
    std::string pathArmorAnimation = "client_src/armoranimation.png";
    std::cout << "Duck loaded: " << static_cast<int>(duckState.color) << " " << duckState.color << std::endl;
    switch (duckState.color) {
    case YELLOW_DUCK:
        pathDuck = "client_src/duckyellow.png";
        pathWings = "client_src/duckyellowflap.png";
        break;
    case GREY_DUCK:
        pathDuck = "client_src/duckgray.png";
        pathWings = "client_src/duckgrayflap.png";
        break;
    case ORANGE_DUCK:
        pathDuck = "client_src/duckorange.png";
        pathWings = "client_src/duckorangeflap.png";
        break;
    default:
        pathDuck = "client_src/duckwhite.png";
        pathWings = "client_src/duckwhiteflap.png";
        break;
    }

    if (!animations[WALKING]->loadTexture(pathDuck)) {
        std::cerr << "Failed to load WALKING texture." << std::endl;
        allLoaded = false;
    }

    if (!animations[JUMPING]->loadTexture(pathDuck)) {
        std::cerr << "Failed to load JUMPING texture." << std::endl;
        allLoaded = false;
    }

    if (!animations[DUCKING]->loadTexture(pathDuck)) {
        std::cerr << "Failed to load DUCKING texture." << std::endl;
        allLoaded = false;
    }

    if (!armorAnimations[WALKING]->loadTexture(pathArmorAnimation)) {
        std::cerr << "Failed to load WALKING texture." << std::endl;
        allLoaded = false;
    }

    if (!armorAnimations[JUMPING]->loadTexture(pathArmorAnimation)) {
        std::cerr << "Failed to load JUMPING texture." << std::endl;
        allLoaded = false;
    }

    if (!armorAnimations[DUCKING]->loadTexture(pathArmorAnimation)) {
        std::cerr << "Failed to load DUCKING texture." << std::endl;
        allLoaded = false;
    }
    
    if(!wings->loadTexture(pathWings)){
        std::cerr << "Failed to load DUCKING texture." << std::endl;
        allLoaded = false;
    }

    if(!weapon->loadTexture()){
        std::cerr << "Failed to load WEAPON texture." << std::endl;
        allLoaded = false;
    }
    
    if (!armor->loadTexture()) {
        std::cerr << "Failed to load chestplate texture." << std::endl;
        allLoaded = false;
    }

    return allLoaded;
}

void Duck::render(const Camera& camera, float zoom) {
    bool faceLeft = (duckState.faceLeft == 1);

    scaleRect.x = duckState.pos.x;
    scaleRect.y = duckState.pos.y;

    //SDL_Rect destRect = scaleRect;

    SDL_Point screenPos = camera.getScreenPosition(scaleRect.x, scaleRect.y, zoom);
    SDL_Rect destRect = {
        screenPos.x,
        screenPos.y,
        static_cast<int>(scaleRect.w * zoom),
        static_cast<int>(scaleRect.h * zoom)
    };

    if (animations[currentAnimation]) {
        animations[currentAnimation]->renderAnimation(destRect.x, destRect.y, destRect, faceLeft, isMoving);

        if (duckState.isFlaping) {
            SDL_Rect scaleFlap;
            scaleFlap.h = SPRITE_FLAP_HEIGHT * 2.5;
            scaleFlap.w = SPRITE_FLAP_WIDTH * 2.5;
            scaleFlap.y = duckState.pos.y + 25;
            scaleFlap.x = duckState.pos.x + 10;
            if (faceLeft) {
                scaleFlap.x = scaleFlap.x + 10;
            }

            SDL_Point screenPosFlap = camera.getScreenPosition(scaleFlap.x, scaleFlap.y, zoom);
            SDL_Rect destRectFlap = {
                screenPosFlap.x,
                screenPosFlap.y,
                static_cast<int>(scaleFlap.w * zoom),
                static_cast<int>(scaleFlap.h * zoom)
            };

            wings->renderAnimation(destRectFlap.x, destRectFlap.y, destRectFlap, faceLeft, true);
        }

        // Renderiza las armaduras si están equipadas
        if (duckState.helmet.type == HELMET_ARMOR) {
            if (!armor->loadTexture())
                std::cout << "Failed to load texture weapon\n"; 
            if (faceLeft)
                armor->render(duckState.pos.x-2, duckState.pos.y - 15, faceLeft, HELMET_ARMOR, camera, zoom);
            else
                armor->render(duckState.pos.x+2 , duckState.pos.y - 15, faceLeft, HELMET_ARMOR, camera, zoom);
        }

        if (duckState.chestplate.type == CHESTPLATE_ARMOR){
            SDL_Rect scaleArmor;
            scaleArmor.h = 32 * 2;
            scaleArmor.w = 32 * 2;
            scaleArmor.y = duckState.pos.y;
            scaleArmor.x = duckState.pos.x;

            SDL_Point screenPosArmor = camera.getScreenPosition(scaleArmor.x, scaleArmor.y, zoom);
            SDL_Rect destRectArmor = {
                screenPosArmor.x,
                screenPosArmor.y,
                static_cast<int>(scaleArmor.w * zoom),
                static_cast<int>(scaleArmor.h * zoom)
            };

            armorAnimations[currentAnimation]->renderAnimation(destRectArmor.x, destRectArmor.y, destRectArmor, faceLeft, isMoving);
        }

        if (weapon->getType() != NULL_WEAPON) {
            if (!weapon->loadTexture())
                std::cout << "Failed to load texture weapon\n"; 
            weapon->render(duckState.pos.x, duckState.pos.y, faceLeft, camera, zoom);
        }

    } else {
        std::cerr << "Attempted to render an uninitialized animation." << std::endl;
    }
}

void Duck::updateState(const duck_t& newDuckState) {
    isMoving = (duckState.pos.x != newDuckState.pos.x || duckState.pos.y != newDuckState.pos.y);
    duckState = newDuckState;
    loadTexture();
    if (duckState.isJumping) {
        currentAnimation = JUMPING;
    } else if (duckState.isDucking) {
        currentAnimation = DUCKING;
    } else {
        currentAnimation = WALKING;
    }

    if (weapon) {
        if (duckState.equipped_weapon.type != weapon->getType()) {
            weapon->updateState(duckState.equipped_weapon);
        }
    }

    if (armor) {
        if (duckState.chestplate.type == CHESTPLATE_ARMOR) {
            armor->updateState(duckState.chestplate);
        }

        if (duckState.helmet.type == HELMET_ARMOR) {
            armor->updateState(duckState.helmet);
        }
    }
}
