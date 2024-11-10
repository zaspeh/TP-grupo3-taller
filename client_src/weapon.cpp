#include "weapon.h"
#include <iostream>

Weapon::Weapon(weapon_t weaponState, SDL_Renderer* renderer)
: weaponState(weaponState)
{
    try {
        guns.emplace(AK_47_WEAPON, std::make_unique<LTexture>(renderer));
        guns.emplace(DARTGUN_WEAPON, std::make_unique<LTexture>(renderer));
        guns.emplace(CHAINSAW_WEAPON, std::make_unique<LTexture>(renderer));
    }catch (const std::bad_alloc& e){
        std::cerr << "Failed to create weapons: " << e.what() << std::endl;
    }
}

bool Weapon::loadTexture(){
    switch (weaponState.type) {
        case AK_47_WEAPON:
            return (guns[AK_47_WEAPON]->loadFromFile("client_src/guns/ak47.png"));
        case DARTGUN_WEAPON:
            return (guns[DARTGUN_WEAPON]->loadFromFile("client_src/guns/dartgun.png"));
        case CHAINSAW_WEAPON:
            return (guns[CHAINSAW_WEAPON]->loadFromFile("client_src/guns/chainsaw.png"));
        default:
            std::cerr << "Error: Invalid weapon type " << static_cast<int>(weaponState.type) << std::endl;
            return false;
    }

}

void Weapon::updateState(const weapon_t& newWeaponState){
    weaponState = newWeaponState;
}

void Weapon::render(float x, float y, bool faceLeft){
    SDL_Rect* scaleRect = new SDL_Rect{0, 0, 0, 0};
    scaleRect->h = guns[DARTGUN_WEAPON]->getHeight() * 2;
    scaleRect->w = guns[DARTGUN_WEAPON]->getWidth() * 2; 
    scaleRect->x = guns[DARTGUN_WEAPON]->getHeight();
    scaleRect->y = guns[DARTGUN_WEAPON]->getWidth(); 
    if(faceLeft){
        x = x-10;
    }else{
        x = x+10;
    }
    SDL_RendererFlip flip = faceLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    guns[DARTGUN_WEAPON]->render(x, y+10, NULL, scaleRect, flip);
}

/* void Weapon::render(float x, float y, bool faceLeft) {
    SDL_Rect scaleRect = {0, 0, 0, 0}; // Declaración directa
    
    auto it = guns.find(weaponState.type);
    if (it != guns.end()) {
        LTexture* texture = it->second.get();  // Acceso seguro
        scaleRect.w = texture->getWidth() * 2;
        scaleRect.h = texture->getHeight() * 2;
        
        SDL_RendererFlip flip = faceLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        texture->render(x + (faceLeft ? -10 : 10), y + 10, nullptr, &scaleRect, flip);
    } else {
        std::cerr << "Weapon texture not found for type: " << static_cast<int>(weaponState.type) << std::endl;
    }
} */

