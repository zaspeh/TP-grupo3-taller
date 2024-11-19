#include "projectile.h"
#include <iostream>

Projectile::Projectile(projectile_t projectileState, SDL_Renderer* renderer)
    : projectileState(projectileState),
      gRenderer(renderer),
      texture(std::make_unique<LTexture>(renderer)) {
    scaleRect.w = SPRITE_PROJECTILE_WIDTH * 2; // Ajustar escala si es necesario
    scaleRect.h = SPRITE_PROJECTILE_HEIGHT * 2;
}

bool Projectile::loadTexture() {
    std::string path = "client_src/guns/granade.png"; // Ruta de la textura

    if (projectileState.type == GRENADE_WEAPON){
        path = "client_src/guns/granade.png";
    }else if(projectileState.type == BANANA_WEAPON){
        path = "client_src/projectiles/bananaprojectile.png";
    }else if(projectileState.type == PEWPEWLASER_WEAPON){
        path = "client_src/projectiles/pewpewlaserprojectile.png";
    }else if(projectileState.type == LASERRIFLE_WEAPON){
        path = "client_src/projectiles/laserrifleprojectile.png";
    }else if(projectileState.type == AK_47_WEAPON){
        path = "client_src/projectiles/ak47projectile.png";
    }else if(projectileState.type == DARTGUN_WEAPON){
        path = "client_src/projectiles/dartgunprojectile.png";
    }else if(projectileState.type == COWBOY_WEAPON){
        path = "client_src/projectiles/cowboyprojectile.png";
    }else if(projectileState.type == MAGNUM_WEAPON){
        path = "client_src/projectiles/magnumprojectile.png";
    }else if(projectileState.type == SHOTGUN_WEAPON){
        path = "client_src/projectiles/shotgunprojectile.png";
    }else{
        path = "client_src/projectiles/sniperprojectile.png";
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
    
    //SDL_Rect destRect = scaleRect;

    SDL_Point screenPos = camera.getScreenPosition(scaleRect.x, scaleRect.y, zoom);
        
    SDL_Rect destRect = {
        screenPos.x,
        screenPos.y,
        static_cast<int>(scaleRect.w * zoom),
        static_cast<int>(scaleRect.h * zoom)
    };

    texture->render(destRect.x + (8*zoom), destRect.y + (15*zoom), nullptr, &destRect, SDL_FLIP_NONE);
}
