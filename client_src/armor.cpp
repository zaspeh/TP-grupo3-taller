#include "armor.h"
#include <iostream>

Armor::Armor(armor_t armorState, SDL_Renderer* renderer) {
    setArmor(armorState);
    try {
        armors.emplace(CHESTPLATE_ARMOR, std::make_unique<LTexture>(renderer));
        armors.emplace(HELMET_ARMOR, std::make_unique<LTexture>(renderer));
    } catch (const std::bad_alloc& e) {
        std::cerr << "Failed to create armor textures: " << e.what() << std::endl;
    }
}

bool Armor::loadTexture() {
    bool success = true;

    if (chestplateState.type != NULL_ARMOR) {
        success &= armors[CHESTPLATE_ARMOR]->loadFromFile("client_src/armors/chessplate.png");
    }
    if (helmetState.type != NULL_ARMOR) {
        success &= armors[HELMET_ARMOR]->loadFromFile("client_src/armors/helmet.png");
    }

    if (!success) {
        std::cerr << "Failed to load one or more armor textures." << std::endl;
    }

    return success;
}

void Armor::render(float x, float y, bool faceLeft) {
    renderArmorPiece(CHESTPLATE_ARMOR, chestplateState, x, y, faceLeft);
    renderArmorPiece(HELMET_ARMOR, helmetState, x, y, faceLeft);
}

void Armor::renderArmorPiece(uint8_t armorType, armor_t& armorState, float x, float y, bool faceLeft) {
    auto it = armors.find(armorType);
    if (it != armors.end() && armorState.type != NULL_ARMOR) {
        LTexture* texture = it->second.get();
        SDL_Rect scaleRect = {0, 0, texture->getWidth() * 2, texture->getHeight() * 2};
        
        SDL_RendererFlip flip = faceLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        texture->render(x + (faceLeft ? -10 : 10), y + 10, nullptr, &scaleRect, flip);
    } else {
        std::cerr << "Texture not found or invalid for armor type: " << static_cast<int>(armorType) << std::endl;
    }
}

void Armor::setArmor(armor_t arm) {
    if (arm.type == CHESTPLATE_ARMOR) {
        chestplateState = arm;
    } else if (arm.type == HELMET_ARMOR) {
        helmetState = arm;
    }
}
