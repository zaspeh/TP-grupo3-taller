#include "weapon.h"
#include <iostream>

Weapon::Weapon(weapon_t weaponState, SDL_Renderer* renderer)
: weaponState(weaponState)
{
    try {
        guns.emplace(0, std::make_unique<LTexture>(renderer));
        guns.emplace(1, std::make_unique<LTexture>(renderer));
        guns.emplace(2, std::make_unique<LTexture>(renderer));
        guns.emplace(3, std::make_unique<LTexture>(renderer));
    }catch (const std::bad_alloc& e){
        std::cerr << "Failed to create weapons: " << e.what() << std::endl;
    }
}

bool Weapon::loadTexture(){
    switch (weaponState.type) {
        case AK_47_WEAPON:
            return (guns[0]->loadFromFile("client_src/guns/ak47.png"));
        case DARTGUN_WEAPON:
            return (guns[0]->loadFromFile("client_src/guns/dartgun.png"));
        case CHAINSAW_WEAPON:
            return (guns[0]->loadFromFile("client_src/guns/chainsaw.png"));
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
    scaleRect->h = guns[0]->getHeight() * 2;
    scaleRect->w = guns[0]->getWidth() * 2; 
    scaleRect->x = guns[0]->getHeight();
    scaleRect->y = guns[0]->getWidth(); 
    if(faceLeft){
        x = x-10;
    }else{
        x = x+10;
    }
    SDL_RendererFlip flip = faceLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    guns[0]->render(x, y+10, NULL, scaleRect, flip);
}
