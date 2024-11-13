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

    if (chestplateState.type != NULL_ARMOR && chestplateState.type == CHESTPLATE_ARMOR) 
        success &= armors[CHESTPLATE_ARMOR]->loadFromFile("client_src/armors/chestplate.png");

    if (helmetState.type != NULL_ARMOR && helmetState.type == HELMET_ARMOR) 
        success &= armors[HELMET_ARMOR]->loadFromFile("client_src/armors/helmet.png");

    if (!success) {
        std::cerr << "Failed to load one or more armor textures." << std::endl;
    }

    return success;
}

void Armor::render(float x, float y, bool faceLeft, uint8_t type) {
    std::cout << "Armors por renderizar\n";
    if (type == CHESTPLATE_ARMOR)
        renderArmorPiece(CHESTPLATE_ARMOR, chestplateState, x, y, faceLeft);
    if (type == HELMET_ARMOR)
        renderArmorPiece(HELMET_ARMOR, helmetState, x, y, faceLeft);
    std::cout << "Armors renderizados\n";
}

void Armor::renderArmorPiece(uint8_t armorType, armor_t& armorState, float x, float y, bool faceLeft) {
    auto it = armors.find(armorType);
    SDL_Rect scaleRect = {0, 0, 0, 0};
    if (it != armors.end() && armorState.type != NULL_ARMOR) {
        LTexture* texture = it->second.get();
        scaleRect.w = texture->getWidth() * 2;
        scaleRect.h = texture->getHeight() * 2;

        SDL_RendererFlip flip = faceLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

        // Desplazamientos específicos para cada pieza de armadura
        float xOffset = 0;
        float yOffset = 0;

        if (armorType == CHESTPLATE_ARMOR) {
            xOffset = 25;  // Ajuste horizontal para el chestplate
            yOffset = 28;  // Ajuste vertical para el chestplate
        } else if (armorType == HELMET_ARMOR) {
            xOffset = 7;   // Ajuste horizontal para el helmet
            yOffset = 0; // Ajuste vertical para el helmet
        }

        // Aplica los desplazamientos
        texture->render(x + xOffset * (faceLeft ? -1 : 1), y + yOffset, nullptr, &scaleRect, flip);
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
    } else if (arm.type == HELMET_ARMOR) {
        helmetState = arm;
    }
}

armor_t Armor::getState() {
    if (chestplateState.type == CHESTPLATE_ARMOR)
        return chestplateState;
    else 
        return helmetState;
}
