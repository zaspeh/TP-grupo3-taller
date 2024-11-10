#include "spawn_place.h"
#include <iostream>

SpawnPlace::SpawnPlace(const spawn_place_t& spawnData, SDL_Renderer* renderer)
    : spawnData(spawnData), renderer(renderer) {
    // Inicializamos la textura a nullptr
    spawnTexture = std::make_unique<LTexture>(renderer);
}

SpawnPlace::~SpawnPlace() {
    // No es necesario hacer nada especial, la textura se libera automáticamente
    std::cout << "SpawnPlace destroyed.\n";
}

bool SpawnPlace::loadTexture() {
    std::string texturePath = "client_src/guns/";  // Directorio base
    
    std::cout << "Weapon type: " << static_cast<int>(spawnData.weapon.type) << std::endl;
    
    switch (spawnData.weapon.type) {
        case AK_47_WEAPON:
            texturePath += "ak47.png";
            break;
        case DARTGUN_WEAPON:
            texturePath += "dartgun.png";
            break;
        case CHAINSAW_WEAPON:
            texturePath += "chainsaw.png";
            break;
        default:
            std::cerr << "Error: Invalid weapon type " << static_cast<int>(spawnData.weapon.type) << std::endl;
            return false;
    }
    
    std::cout << "Attempting to load texture from: " << texturePath << std::endl;
    
    // Verificar que el archivo existe antes de intentar cargarlo
    FILE* file = fopen(texturePath.c_str(), "r");
    if (file == nullptr) {
        std::cerr << "Error: File does not exist: " << texturePath << std::endl;
        return false;
    }
    fclose(file);
    
    if (!spawnTexture->loadFromFile(texturePath.c_str())) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
        std::cerr << "SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    
    std::cout << "Successfully loaded texture: " << texturePath << std::endl;
    return true;
}

void SpawnPlace::render() {
    if (spawnTexture) {
        // Posición original
        int originalWidth = spawnTexture->getWidth();
        int originalHeight = spawnTexture->getHeight();
        
        int x = spawnData.weapon.pos.x;
        int y = spawnData.weapon.pos.y;
        
        // Aumentar el tamaño en un factor de 2
        int scaledWidth = static_cast<int>(originalWidth * 2);
        int scaledHeight = static_cast<int>(originalHeight * 2);

        y += 95;

        SDL_Rect destRect = {x, y, scaledWidth, scaledHeight};
        spawnTexture->render(x, y, nullptr, &destRect);
    }
}



position_t SpawnPlace::getPosition() const {
    return spawnData.pos;
}

bool SpawnPlace::isActive() const {
    return spawnData.is_active;
}
