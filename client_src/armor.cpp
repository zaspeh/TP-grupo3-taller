#include "armor.h"
#include <iostream>

Armor::Armor(armor_t armorState, SDL_Renderer* renderer) {
    updateState(armorState);
    try {
        armors.emplace(CHESTPLATE_ARMOR, std::make_unique<LTexture>(renderer));
        armors.emplace(HELMET_ARMOR, std::make_unique<LTexture>(renderer));
    } catch (const std::bad_alloc& e) {
        std::cerr << "Failed to create armor textures: " << e.what() << std::endl;
    }
}

bool Armor::loadTexture() {
    bool success = true;

    if (chestplateState.type != NULL_ARMOR) 
        success &= armors[CHESTPLATE_ARMOR]->loadFromFile("client_src/armors/chestplate.png");

    if (helmetState.type != NULL_ARMOR) 
        success &= armors[HELMET_ARMOR]->loadFromFile("client_src/armors/helmet.png");

        

    if (!success) {
        std::cerr << "Failed to load one or more armor textures." << std::endl;
    }

    return success;
}

void Armor::render(int x, int y, bool faceLeft, uint8_t type, const Camera& camera, float zoom) {
    if (type == CHESTPLATE_ARMOR)
        renderArmorPiece(CHESTPLATE_ARMOR, chestplateState, x, y, faceLeft, camera, zoom);
    if (type == HELMET_ARMOR)
        renderArmorPiece(HELMET_ARMOR, helmetState, x, y, faceLeft, camera, zoom);
}

void Armor::renderArmorPiece(uint8_t armorType, armor_t& armorState, int x, int y, bool faceLeft, const Camera& camera, float zoom) {
    auto it = armors.find(armorType);
    SDL_Rect scaleRect = {x, y, 0, 0};
    if (it != armors.end() && armorType != NULL_ARMOR) {
        LTexture* texture = it->second.get();
        scaleRect.w = (texture->getWidth()-1) * 2.1;
        scaleRect.h = (texture->getHeight()-1) * 2.1;

        SDL_RendererFlip flip = faceLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        
        SDL_Point screenPos = camera.getScreenPosition(scaleRect.x, scaleRect.y, zoom);
        
        SDL_Rect destRect = {
            screenPos.x,
            screenPos.y,
            static_cast<int>(scaleRect.w * zoom),
            static_cast<int>(scaleRect.h * zoom)
        };

        texture->render(destRect.x, destRect.y, nullptr, &destRect, flip);
    } else {
        std::cerr << "Texture not found or invalid for armor type: " << static_cast<int>(armorType) << std::endl;
    }
}

int Armor::getType() {
    if (chestplateState.type == CHESTPLATE_ARMOR) {
        return CHESTPLATE_ARMOR;
    } else if (helmetState.type == HELMET_ARMOR) {
        return HELMET_ARMOR;
    }
    return NULL_ARMOR;
}

void Armor::updateState(armor_t arm) {
    if (arm.type == CHESTPLATE_ARMOR) {
        chestplateState = arm;
        if (helmetState.type == NULL_ARMOR)
            helmetState = nullArmor;
    }
    if (arm.type == HELMET_ARMOR) {
        helmetState = arm;
        if (chestplateState.type == NULL_ARMOR)
            chestplateState = nullArmor;
    }
}

armor_t Armor::getState() {
    
    if (chestplateState.type == CHESTPLATE_ARMOR)
        return chestplateState;
    else if (helmetState.type == HELMET_ARMOR)
        return helmetState;
    else 
        return nullArmor;
}
