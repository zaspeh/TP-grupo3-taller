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
    if (!texture->loadFromFile(path)) {
        std::cerr << "Failed to load projectile texture." << std::endl;
        return false;
    }
    return true;
}

void Projectile::updateState(const projectile_t& newState) {
    projectileState = newState;
}

void Projectile::render() {
    texture->render(projectileState.pos.x + 8, projectileState.pos.y + 15, nullptr, &scaleRect, SDL_FLIP_NONE);
}
