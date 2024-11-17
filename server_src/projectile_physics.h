#ifndef PROJECTILE_PHYSICS_H
#define PROJECTILE_PHYSICS_H

#include <cmath>
#include <algorithm>
#include "../common_src/utils.h"
#include "level.h"
#include "box.h"

// Forward declarations
class GameState;

class ProjectilePhysics {
private:
    float velocity;
    float velocityX;    // Nueva variable para velocidad horizontal
    float velocityY;    // Nueva variable para velocidad vertical
    float angle;
    float initialX;
    bool isActive;
    float gravity = 980.0f;

public:
    ProjectilePhysics() :
        velocity(0.0f),
        velocityX(0.0f),
        velocityY(0.0f),
        angle(0.0f),
        isActive(false) {}

    void initProjectile(float initialVelocity, float initialAngle, float initialX, float gravit) {
        this->initialX = initialX;
        this->velocity = initialVelocity;
        this->angle = initialAngle;
        this->gravity = gravit;
        // Inicializar componentes de velocidad
        this->velocityX = initialVelocity * std::cos(angle);
        this->velocityY = initialVelocity * std::sin(angle);
        isActive = true;
    }

    bool updatePosition(projectile_t &projectile, level_t& levelState, 
                       float deltaTime, float distance, GameState* gameState) {
        if (!projectile.is_active) {
            return false;
        }
        
        deltaTime = std::min(deltaTime, 0.016f);

        if (projectile.type == GRENADE_WEAPON || projectile.type == BANANA_WEAPON) {
            // Actualizar velocidad vertical con gravedad
            velocityY += gravity * deltaTime;
            
            // Actualizar posición con las componentes de velocidad
            projectile.pos.x += velocityX * deltaTime;
            projectile.pos.y += velocityY * deltaTime;
        } else {
            // Para otros proyectiles mantener el comportamiento original
            projectile.pos.x += velocity * std::cos(angle) * deltaTime;
            projectile.pos.y += velocity * std::sin(angle) * deltaTime;
        }

        // El resto del código de colisiones se mantiene igual
        // Colisiones con plataformas
        for (int i = 0; i < levelState.num_platforms; i++) {
            bool horizontalOverlap = (projectile.pos.x >= levelState.platforms[i].pos.x) && 
                                   (projectile.pos.x <= levelState.platforms[i].pos.x + WIDTH_PLATFORM);
            bool verticalOverlap = (projectile.pos.y >= levelState.platforms[i].pos.y) && 
                                  (projectile.pos.y <= levelState.platforms[i].pos.y + HEIGHT_PLATFORM);

            if (horizontalOverlap && verticalOverlap) {
                return false;
            }
        }

        // Verificar límites y distancia
        if (projectile.pos.x < 0 || projectile.pos.x > LEVEL_WIDTH || 
            projectile.pos.y < 0 || projectile.pos.y > LEVEL_HEIGHT ||
            abs(projectile.pos.x - initialX) > distance * 32) {
            return false;
        }

        return true;
    }

    bool isProjectileActive() const { 
        return isActive; 
    }
};

#endif // PROJECTILE_PHYSICS_H