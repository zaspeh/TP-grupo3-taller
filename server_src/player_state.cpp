#include "player_state.h"

bool PlayerState::doNotCollideX(platform_t* plat, uint8_t numPlats, int new_x) {
    for (int i = 0; i < numPlats; i++) {
        // Intersección en Y (para evitar plataformas "fantasma")
        if (duck.pos.y + HEIGHT_DUCK > plat[i].pos.y && 
            duck.pos.y < plat[i].pos.y + HEIGHT_PLATFORM) {

            // Validar correctamente si estamos dentro de la plataforma en X
            if (new_x > plat[i].pos.x && 
                new_x < plat[i].pos.x + WIDTH_PLATFORM) {
                return false;
            }
        }
    }
    return true;
}

void PlayerState::move(int dx, int dy, platform_t* plat, uint8_t numPlats) {
    // Movimiento horizontal
    if (dx != 0) {
        int newX = duck.pos.x + dx;
        if (doNotCollideX(plat, numPlats, newX)) {
            duck.pos.x = newX;
        }
    }
}

void PlayerState::updatePosition(float deltaTime, platform_t* platforms, uint8_t numPlatforms) {
    deltaTime = std::min(deltaTime, 0.016f);
    
    verticalVelocity += gravity * deltaTime;
    float newY = duck.pos.y + (verticalVelocity * deltaTime);
    
    isOnGround = false;
    bool hitCeiling = false;
    
    for (int i = 0; i < numPlatforms; i++) {
        // Verificación más precisa de la intersección horizontal
        bool horizontalOverlap = (duck.pos.x + WIDTH_DUCK > platforms[i].pos.x + 20) && // Añadimos un pequeño margen
                                (duck.pos.x < platforms[i].pos.x + WIDTH_PLATFORM);  // para evitar colisiones fantasma
        
        if (horizontalOverlap) {
            // Colisión con el suelo - añadimos un margen de tolerancia
            if (newY + HEIGHT_DUCK > platforms[i].pos.y && 
                duck.pos.y + HEIGHT_DUCK <= platforms[i].pos.y + 10) { // Margen de tolerancia
                duck.pos.y = platforms[i].pos.y - HEIGHT_DUCK;
                verticalVelocity = 0;
                isOnGround = true;
                duck.isJumping = false;
                duck.isFalling = false;
                break;
            }
            // Colisión con el techo - mejoramos la detección
            else if (newY < platforms[i].pos.y + HEIGHT_PLATFORM && 
                     duck.pos.y >= platforms[i].pos.y + HEIGHT_PLATFORM - 5) { // Reducimos el margen de colisión
                if (duck.isJumping || verticalVelocity < 0) {
                    duck.pos.y = platforms[i].pos.y + HEIGHT_PLATFORM;
                    verticalVelocity = 0;
                    hitCeiling = true;
                    duck.isJumping = false;
                    duck.isFalling = true;
                }
                break;
            }
        }
    }

    // Si no hay colisiones, actualizar la posición
    if (!isOnGround && !hitCeiling) {
        duck.pos.y = newY;
        duck.isFalling = verticalVelocity > 0;
    }
    
    const float maxFallSpeed = 800.0f;
    if (verticalVelocity > maxFallSpeed) {
        verticalVelocity = maxFallSpeed;
    }
}

void PlayerState::jump() {
    // Solo permitir saltar si estamos en el suelo
    if (isOnGround) {
        duck.isJumping = true;
        duck.isFalling = false;
        verticalVelocity = jumpStrength;
        isOnGround = false;  // Inmediatamente nos quitamos del suelo
    }
}

void PlayerState::reload(uint8_t newAmmo) { 
    weapon->reload(newAmmo);
}

void PlayerState::takeDamage(uint8_t damage) {
    if (armor.isEquipped()) {
        armor.absorb_hit();
        return;
    }
    if (helmet.isEquipped()) {
        helmet.absorb_hit();
        return;
    }
    
    duck.health -= damage;
    if (duck.health <= 0) {
        setAlive();
    }
}

void PlayerState::pickWeapon(Weapon* newWeapon) {
    if (weapon) {
        std::cout << "Dropping weapon\n";
        dropWeapon();
    }
    weapon = newWeapon;
    duck.equipped_weapon.type = newWeapon->getId();
    std::cout << "Tipo de arma: " << static_cast<int>(duck.equipped_weapon.type) << std::endl;
}

void PlayerState::dropWeapon() {
    if (weapon != nullptr) {
        weapon = nullptr;
        duck.equipped_weapon.type = NULL_WEAPON;
    }
}

void PlayerState::shoot(){
    if (weapon != nullptr && !weapon->canShoot()) {
        weapon->shoot(); // se hacen los cambios de balas y eso
    }
}
