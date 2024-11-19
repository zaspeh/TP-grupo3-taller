#include "spawn_place.h"
#include <iostream>

SpawnPlace::SpawnPlace(const spawn_place_t& spawnData, SDL_Renderer* renderer)
    : spawnData(spawnData), renderer(renderer) {
    // Inicializamos la textura a nullptr
    spawnTexture = std::make_unique<LTexture>(renderer);

    if (spawnData.weapon.type != NULL_WEAPON) {
        weapon = std::make_unique<Weapon>(spawnData.weapon, renderer);
    } else if (spawnData.armor.type != NULL_ARMOR){
        armor = std::make_unique<Armor>(spawnData.armor, renderer);
    }
}

bool SpawnPlace::loadTexture() {
    bool success = true;

    if (!spawnTexture->loadFromFile("client_src/spawn/spawnplace.png")) {
        std::cerr << "Failed to load spawn texture.\n";
        success = false;
    }

    if (weapon && !weapon->loadTexture()) {
        std::cerr << "Failed to load weapon texture.\n";
        success = false;
    }

    if (armor && !armor->loadTexture()) {
        std::cerr << "Failed to load armor texture.\n";
        success = false;
    }

    return success;
}

void SpawnPlace::render(const Camera& camera, float zoom) {
    if (spawnTexture) {
        int originalWidth = spawnTexture->getWidth();
        int originalHeight = spawnTexture->getHeight();
        int x = spawnData.pos.x + 20;

        int platformOffset = 50;
        int y = spawnData.pos.y + platformOffset;
        int scaledWidth = static_cast<int>(originalWidth * 2.5);
        int scaledHeight = static_cast<int>(originalHeight * 2.5);

        SDL_Point screenPos = camera.getScreenPosition(x, y, zoom);
        
        SDL_Rect destRect = {
            screenPos.x,
            screenPos.y,
            static_cast<int>(scaledWidth * zoom),
            static_cast<int>(scaledHeight * zoom)
        };

        spawnTexture->render(destRect.x, destRect.y, nullptr, &destRect, SDL_FLIP_NONE);
    }

    if (spawnData.weapon.type != NULL_WEAPON) 
        weapon->render(spawnData.weapon.pos.x, spawnData.weapon.pos.y, false, camera, zoom);
    
    if (spawnData.armor.type != NULL_ARMOR)
        armor->render(spawnData.armor.pos.x+(zoom), spawnData.armor.pos.y+(zoom), false, spawnData.armor.type, camera, zoom);
}

void SpawnPlace::updateState(const spawn_place_t& newState) {
    spawnData = newState;


    if (newState.weapon.type != NULL_WEAPON) {
        if (weapon == nullptr || weapon->getType() != newState.weapon.type) {
            weapon = std::make_unique<Weapon>(newState.weapon, renderer);
            if (!weapon->loadTexture()) {
                std::cerr << "Failed to load new weapon texture.\n";
            }
        } else {
            weapon->updateState(newState.weapon);
        }
    } else {
        weapon = nullptr;
    }

    if (newState.armor.type != NULL_ARMOR) {
        if (armor == nullptr || armor->getType() != newState.armor.type) {
            armor = std::make_unique<Armor>(newState.armor, renderer);
            if (!armor->loadTexture()) {
                std::cerr << "Failed to load new armor texture.\n";
            }
        } else {
            armor->updateState(newState.armor);
        }
    } else {
        armor = nullptr;
    }


}

position_t SpawnPlace::getPosition() const {
    return spawnData.pos;
}

bool SpawnPlace::isActive() const {
    return spawnData.is_active;
}

SpawnPlace::~SpawnPlace() {
    std::cout << "SpawnPlace destroyed.\n";
}