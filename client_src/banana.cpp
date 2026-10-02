#include "banana.h"

Banana::Banana(SDL_Renderer* renderer) : 
    renderer(renderer),
    texture(std::make_unique<LTexture>(renderer)),
    position({0, 0}) {
}

bool Banana::loadTexture() {
    bool charged = texture->loadFromFile("client_src/projectiles/bananaprojectile.png");
    scaleRect.h = texture->getHeight() * 2;
    scaleRect.w = texture->getWidth() * 2;
    return charged;
}

void Banana::render(const Camera& camera, float zoom) {
    if (!texture) return;
    scaleRect.x = position.x;
    scaleRect.y = position.y;
    
    SDL_Point screenPos = camera.getScreenPosition(scaleRect.x, scaleRect.y, zoom);

    SDL_Rect destRect = {
        screenPos.x,
        screenPos.y,
        static_cast<int>(scaleRect.w * zoom),
        static_cast<int>(scaleRect.h * zoom)
    };

    texture->render(destRect.x, destRect.y, nullptr, &destRect, SDL_FLIP_NONE);
}

void Banana::updatePosition(const position_t& pos) {
    position = pos;
}