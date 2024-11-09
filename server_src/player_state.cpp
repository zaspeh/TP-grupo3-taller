#include "player_state.h"

// Other methods
/*void PlayerState::move(int dx, int dy, platform_t* plat, uint8_t numPlats) {
    if (doNotCollideX(plat, numPlats, duck.pos.x + dx)) {
        duck.pos.x += dx;
    }
    if(doNotCollideY(plat, numPlats, duck.pos.y + dy)) {
        duck.pos.y += dy;
    }
}*/

bool PlayerState::doNotCollideX(platform_t* plat, uint8_t numPlats, int new_x) {
    for (int i = 0; i < numPlats; i++) {
        if (duck.pos.y + 32 > plat[i].platform.y && 
            duck.pos.y < plat[i].platform.y + 32) {
            
            if (new_x < plat[i].platform.x + 32 &&
                new_x + 32 > plat[i].platform.x) {
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
    
    // Aplicar gravedad siempre
    verticalVelocity += gravity * deltaTime;
    
    // Calcular nueva posición Y
    float newY = duck.pos.y + (verticalVelocity * deltaTime);
    
    // Verificar colisión con plataformas
    isOnGround = false;  // Reset al inicio de cada frame
    
    for (int i = 0; i < numPlatforms; i++) {
        // Verificar si estamos sobre la plataforma en X
        if (duck.pos.x + 32 > platforms[i].platform.x && 
            duck.pos.x < platforms[i].platform.x + 32) {
            
            // Colisión con plataforma
            if (newY + 32 > platforms[i].platform.y) {
                // Si estamos cayendo
                if (verticalVelocity > 0 && duck.pos.y + 32 <= platforms[i].platform.y) {
                    duck.pos.y = platforms[i].platform.y - 32;
                    verticalVelocity = 0;
                    isOnGround = true;
                    duck.isJumping = false;
                    duck.isFalling = false;
                    break;
                }
                // Si golpeamos una plataforma mientras subimos
                else if (verticalVelocity < 0) {
                    verticalVelocity = 0;
                    newY = duck.pos.y; // Mantener posición actual
                }
            }
        }
    }

        // Si no estamos en el suelo, actualizar posición Y
        if (!isOnGround) {
            duck.pos.y = newY;
            duck.isFalling = verticalVelocity > 0;
        }
        
        // Limitar la velocidad máxima de caída
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
        dropWeapon();
    }
    weapon = newWeapon;
    duck.equipped_weapon.is_equipped = true;
    duck.equipped_weapon.type = newWeapon->getId();
    duck.equipped_weapon.ammo = newWeapon->getAmmo();
}

void PlayerState::dropWeapon() {
    if (weapon != nullptr) {
        duck.equipped_weapon.is_equipped = false;
        weapon = nullptr;
    }
}

void PlayerState::shoot(){
    if (weapon != nullptr && !weapon->canShoot()) {
        weapon->shoot(); // se hacen los cambios de balas y eso
        duck.equipped_weapon.ammo = weapon->getAmmo();
    }
}
