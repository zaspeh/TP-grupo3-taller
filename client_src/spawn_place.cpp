#include "spawn_place.h"
#include <iostream>


SpawnPlace::SpawnPlace(const spawn_place_t& spawnData, SDL_Renderer* renderer)
    : spawnData(spawnData), renderer(renderer) {
    // Inicializamos la textura a nullptr
    spawnTexture = std::make_unique<LTexture>(renderer);

    if (spawnData.weapon.type != NULL_WEAPON) {
        weapon = std::make_unique<Weapon>(spawnData.weapon, renderer);
    }
    
    //if(spawnData.armor.type != NULL_ARMOR){
    //    armor = std::make_unique<Armor>(spawnData.armor, renderer);
    ///}
    //std::cout << "SpawnPlace created.\n";
}

SpawnPlace::~SpawnPlace() {
    // No es necesario hacer nada especial, la textura se libera automáticamente
    std::cout << "SpawnPlace destroyed.\n";
}

bool SpawnPlace::loadTexture() {
    bool success = true;

    if (!spawnTexture->loadFromFile("client_src/spawn/spawnplace.png")) {
        std::cerr << "Failed to load spawn texture.\n";
        success = false;
    }

    if (!weapon->loadTexture()) {
        std::cerr << "Failed to load weapon texture.\n";
        success = false;
    }
    return success;
}

void SpawnPlace::render() {
    if (spawnTexture) {
        int originalWidth = spawnTexture->getWidth();
        int originalHeight = spawnTexture->getHeight();
        int x = spawnData.pos.x + 20;
        // Ajustamos la posición Y de la plataforma 20 píxeles más abajo
        int platformOffset = 50;
        int y = spawnData.pos.y + platformOffset;
        int scaledWidth = static_cast<int>(originalWidth * 2.5);
        int scaledHeight = static_cast<int>(originalHeight * 2.5);
        SDL_Rect destRect = {x, y, scaledWidth, scaledHeight};
        spawnTexture->render(x, y, nullptr, &destRect);
    }

    // El arma se mantiene en su posición original
    if (spawnData.weapon.type != NULL_WEAPON) {
        weapon->render(spawnData.weapon.pos.x, spawnData.weapon.pos.y, false);
    }

    if (spawnData.armor.type != NULL_ARMOR) {
        //helmetTexture->render(spawnData.weapon.pos.x, spawnData.weapon.pos.y, false);
    }
}

void SpawnPlace::updateState(const spawn_place_t& newState) {
    spawnData = newState;

    // Manejar cambios en el arma
    if (newState.weapon.type != NULL_WEAPON) {
        std::cout << "Actualizando el arma\n";
        if (weapon == nullptr || weapon->getType() != newState.weapon.type) {
            std::cout << "Creando una nueva arma\n";
            // Si el arma no existe o es diferente, creamos una nueva
            weapon = std::make_unique<Weapon>(newState.weapon, renderer);
            if (!weapon->loadTexture()) {
                std::cerr << "Failed to load new weapon texture.\n";
            }
        } else {
            std::cout << "El arma ya existe\n";
            // Si es la misma arma, solo actualizamos su estado
            weapon->updateState(newState.weapon);
        }
    } else {
        // Si el nuevo estado no tiene arma, eliminamos cualquier arma existente
        std::cout << "Eliminando arma del spawn\n";
        weapon = nullptr;
    }

    //std::cout << "SpawnPlace state updated.\n";
}

position_t SpawnPlace::getPosition() const {
    return spawnData.pos;
}

bool SpawnPlace::isActive() const {
    return spawnData.is_active;
}
