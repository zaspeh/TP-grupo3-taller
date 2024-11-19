#include "projectile.h"
#include <iostream>

Projectile::Projectile(projectile_t projectileState, SDL_Renderer* renderer)
    : projectileState(projectileState),
      gRenderer(renderer),
      texture(std::make_unique<LTexture>(renderer)) {
    scaleRect.w = SPRITE_PROJECTILE_WIDTH * 2; 
    scaleRect.h = SPRITE_PROJECTILE_HEIGHT * 2;
}

bool Projectile::loadTexture() {
    std::string path = "client_src/projectiles/"; 

    switch (projectileState.type)
    {
    case GRENADE_WEAPON:
        path =  "client_src/guns/granade.png";
        break;
    case BANANA_WEAPON:
        path += "bananaprojectile.png";
        break;
    case PEWPEWLASER_WEAPON:
        path += "pewpewlaserprojectile.png";
        break;
    case LASERRIFLE_WEAPON:
        path += "laserrifleprojectile.png";
        break;
    case AK_47_WEAPON:
        path += "ak47projectile.png";
        break;
    case DARTGUN_WEAPON:
        path += "dartgunprojectile.png";
        break;
    case COWBOY_WEAPON:
        path += "cowboyprojectile.png";
        break;
    case MAGNUM_WEAPON:
        path += "magnumprojectile.png";
        break;
    case SHOTGUN_WEAPON:
        path += "shotgunprojectile.png";
        break;
    case SNIPER_WEAPON:
        path += "sniperprojectile.png";
        break;
    default:
        break;
    }

    if (!texture->loadFromFile(path)) {
        std::cerr << "Failed to load projectile texture." << std::endl;
        return false;
    }
    return true;
}

void Projectile::updateState(const projectile_t& newState) {
    projectileState = newState;
}

void Projectile::render(const Camera& camera, float zoom) {

    scaleRect = {0, 0, 0, 0};
    scaleRect.h = texture->getHeight()*2;
    scaleRect.w = texture->getWidth()*2; 
    scaleRect.x = projectileState.pos.x;
    scaleRect.y = projectileState.pos.y; 
    
    SDL_Point screenPos = camera.getScreenPosition(scaleRect.x, scaleRect.y, zoom);
        
    SDL_Rect destRect = {
        screenPos.x,
        screenPos.y,
        static_cast<int>(scaleRect.w * zoom),
        static_cast<int>(scaleRect.h * zoom)
    };

    texture->render(destRect.x + (8*zoom), destRect.y + (15*zoom), nullptr, &destRect, SDL_FLIP_NONE);
}
