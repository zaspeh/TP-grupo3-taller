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

/*
Initialized ducks vector with 1 ducks.
Initialized ducks vector with 114 platforms.
Tamaño de spawns: 8
Creando nuevo spawn: 6
Creando nuevo spawn: 0
Creando nuevo spawn: 4
Creando nuevo spawn: 0
Creando nuevo spawn: 4
Creando nuevo spawn: 0
Cambiando el tamaño de las cajas
Cajas reziseadas.
Cargando boxes
Cargando boxes
Caja sin vida: client_src/spawn/
Unable to load image client_src/spawn/! SDL_image Error: Unsupported image format
Failed to load box texture from client_src/spawn/
SDL_image Error: Unsupported image format
Failed to load texture for box 1.
Failed to initialize game or load media.
^C^C^C^A^CTerminado (killed)

*/

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
        // Ajustamos la posición Y de la plataforma 20 píxeles más abajo
        int platformOffset = 50;
        int y = spawnData.pos.y + platformOffset;
        int scaledWidth = static_cast<int>(originalWidth * 2.5);
        int scaledHeight = static_cast<int>(originalHeight * 2.5);
        //SDL_Rect destRect = {x, y, scaledWidth, scaledHeight};

        SDL_Point screenPos = camera.getScreenPosition(x, y, zoom);
        
        SDL_Rect destRect = {
            screenPos.x,
            screenPos.y,
            static_cast<int>(scaledWidth * zoom),
            static_cast<int>(scaledHeight * zoom)
        };

        spawnTexture->render(x, y, nullptr, &destRect, SDL_FLIP_NONE);
    }

    // El arma se mantiene en su posición original
    if (spawnData.weapon.type != NULL_WEAPON) 
        weapon->render(spawnData.weapon.pos.x, spawnData.weapon.pos.y, false, camera, zoom);
    
    if (spawnData.armor.type != NULL_ARMOR)
        armor->render(spawnData.armor.pos.x, spawnData.armor.pos.y, false, spawnData.armor.type, camera, zoom);
}

void SpawnPlace::updateState(const spawn_place_t& newState) {
    spawnData = newState;

    // Manejar cambios en el arma
    if (newState.weapon.type != NULL_WEAPON) {
        if (weapon == nullptr || weapon->getType() != newState.weapon.type) {
            // Si el arma no existe o es diferente, creamos una nueva
            weapon = std::make_unique<Weapon>(newState.weapon, renderer);
            if (!weapon->loadTexture()) {
                std::cerr << "Failed to load new weapon texture.\n";
            }
        } else {
            // Si es la misma arma, solo actualizamos su estado
            weapon->updateState(newState.weapon);
        }
    } else {
        weapon = nullptr;
    }

    if (newState.armor.type != NULL_ARMOR) {
        if (armor == nullptr || armor->getType() != newState.armor.type) {
            // Si el arma no existe o es diferente, creamos una nueva
            armor = std::make_unique<Armor>(newState.armor, renderer);
            if (!armor->loadTexture()) {
                std::cerr << "Failed to load new armor texture.\n";
            }
        } else {
            // Si es la misma arma, solo actualizamos su estado
            armor->updateState(newState.armor);
        }
    } else {
        armor = nullptr;
    }

    //std::cout << "SpawnPlace state updated.\n";
}

position_t SpawnPlace::getPosition() const {
    return spawnData.pos;
}

bool SpawnPlace::isActive() const {
    return spawnData.is_active;
}
