#include "weapon.h"
#include <iostream>

Weapon::Weapon(weapon_t weaponState, SDL_Renderer* renderer)
: weaponState(weaponState)
{
    try {
        guns.emplace(AK_47_WEAPON, std::make_unique<LTexture>(renderer));
        guns.emplace(DARTGUN_WEAPON, std::make_unique<LTexture>(renderer));
        guns.emplace(BANANA_WEAPON, std::make_unique<LTexture>(renderer));
        guns.emplace(GRENADE_WEAPON, std::make_unique<LTexture>(renderer));
        guns.emplace(PEWPEWLASER_WEAPON, std::make_unique<LTexture>(renderer));
        guns.emplace(LASERRIFLE_WEAPON, std::make_unique<LTexture>(renderer));
        guns.emplace(COWBOY_WEAPON, std::make_unique<LTexture>(renderer));
        guns.emplace(MAGNUM_WEAPON, std::make_unique<LTexture>(renderer));
        guns.emplace(SHOTGUN_WEAPON, std::make_unique<LTexture>(renderer));
        guns.emplace(SNIPER_WEAPON, std::make_unique<LTexture>(renderer));

        weaponYOffsets = {
            {AK_47_WEAPON, 10},
            {DARTGUN_WEAPON, 15},
            {BANANA_WEAPON, 5},
            {GRENADE_WEAPON, 8},
            {PEWPEWLASER_WEAPON, 12},
            {LASERRIFLE_WEAPON, 10},
            {COWBOY_WEAPON, 30},
            {MAGNUM_WEAPON, 6},
            {SHOTGUN_WEAPON, 25},
            {SNIPER_WEAPON, 11}
        };

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
        case BANANA_WEAPON:
            return (guns[BANANA_WEAPON]->loadFromFile("client_src/guns/banana.png"));
        case GRENADE_WEAPON:
            return (guns[GRENADE_WEAPON]->loadFromFile("client_src/guns/granade.png"));
        case PEWPEWLASER_WEAPON:
            return (guns[PEWPEWLASER_WEAPON]->loadFromFile("client_src/guns/pewpewlaser.png"));
        case LASERRIFLE_WEAPON:
            return (guns[LASERRIFLE_WEAPON]->loadFromFile("client_src/guns/laserrifle.png"));
        case COWBOY_WEAPON:
            return (guns[COWBOY_WEAPON]->loadFromFile("client_src/guns/cowboypistol.png"));
        case MAGNUM_WEAPON:
            return (guns[MAGNUM_WEAPON]->loadFromFile("client_src/guns/magnum.png"));
        case SHOTGUN_WEAPON:
            return (guns[SHOTGUN_WEAPON]->loadFromFile("client_src/guns/shotgun.png"));
        case SNIPER_WEAPON:
            return (guns[SNIPER_WEAPON]->loadFromFile("client_src/guns/sniper.png"));
        default:
            std::cerr << "Error: Invalid weapon type " << static_cast<int>(weaponState.type) << std::endl;
            return true;
    }

}

void Weapon::updateState(const weapon_t& newWeaponState){
    weaponState = newWeaponState;
}

void Weapon::render(int x, int y, bool faceLeft, const Camera& camera, float zoom) {
    SDL_Rect scaleRect = {x, y, 0, 0};
    auto it = guns.find(weaponState.type);
    if (it != guns.end()) {
        LTexture* texture = it->second.get();
        scaleRect.w = texture->getWidth() * 2;
        scaleRect.h = texture->getHeight() * 2;
        
        // Obtener el offset Y específico para esta arma
        int yOffset = weaponYOffsets[weaponState.type];

        //SDL_Rect destRect = scaleRect;

        SDL_Point screenPos = camera.getScreenPosition(scaleRect.x, scaleRect.y, zoom);
        
        SDL_Rect destRect = {
            screenPos.x,
            screenPos.y,
            static_cast<int>(scaleRect.w * zoom),
            static_cast<int>(scaleRect.h * zoom)
        };
        
        SDL_RendererFlip flip = faceLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        texture->render(
            destRect.x + ((faceLeft ? -10 : 10)*zoom), 
            destRect.y + (yOffset*zoom),  // Usar el offset específico del arma
            nullptr, 
            &destRect, 
            flip
        );
    } else {
        std::cerr << "Weapon texture not found for type: " << static_cast<int>(weaponState.type) << std::endl;
    }
}
