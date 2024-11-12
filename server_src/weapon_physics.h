#ifndef WEAPON_PHYSICS_H
#define WEAPON_PHYSICS_H

#include <cmath>
#include <algorithm>
#include "../common_src/utils.h"
#include "../common_src/game_state.h"

class WeaponPhysics {
private:
    float verticalVelocity;
    float gravity;
    bool isOnGround;
    
    static constexpr float WEAPON_HEIGHT = 20.0f;
    static constexpr float WEAPON_WIDTH = 20.0f;

public:
    WeaponPhysics() : 
        verticalVelocity(0.0f),
        gravity(980.0f),
        isOnGround(false) {}

    bool updatePosition(weapon_t& weapon, float deltaTime, platform_t* platforms, uint8_t numPlatforms) {
        deltaTime = std::min(deltaTime, 0.016f);
        
        verticalVelocity += gravity * deltaTime;
        float newY = weapon.pos.y + (verticalVelocity * deltaTime);
        
        isOnGround = false;
        
        for (int i = 0; i < numPlatforms; i++) {
            bool horizontalOverlap = (weapon.pos.x + WEAPON_WIDTH > platforms[i].pos.x) && 
                                   (weapon.pos.x < platforms[i].pos.x + WIDTH_PLATFORM);
            
            if (horizontalOverlap) {
                // Colisión con el suelo
                if (newY + WEAPON_HEIGHT > platforms[i].pos.y && 
                    weapon.pos.y + WEAPON_HEIGHT <= platforms[i].pos.y + 5) {
                    weapon.pos.y = platforms[i].pos.y - WEAPON_HEIGHT;
                    verticalVelocity = 0;
                    isOnGround = true;
                    return true; // El arma ha llegado a una plataforma
                }
            }
        }

        // Si no hay colisiones, actualizar la posición
        if (!isOnGround) {
            weapon.pos.y = newY;
        }

        // Limitar la velocidad máxima de caída
        float maxFallSpeed = 800.0f;
        if (verticalVelocity > maxFallSpeed) {
            verticalVelocity = maxFallSpeed;
        }

        return false; // El arma sigue cayendo
    }
};

#endif // WEAPON_PHYSICS_H