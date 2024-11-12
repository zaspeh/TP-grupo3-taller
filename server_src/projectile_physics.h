#ifndef PROJECTILE_PHYSICS_H
#define PROJECTILE_PHYSICS_H

#include <cmath>
#include <algorithm>
#include "../common_src/utils.h"
#include "../common_src/game_state.h"

class ProjectilePhysics {
private:
    float initialX;
    float velocity;
    float acceleration;
    float angle;
    bool isActive;

public:
    ProjectilePhysics() :
        velocity(0.0f),
        acceleration(0.0f),
        angle(0.0f),
        isActive(false) {}

    void initProjectile(float initialVelocity, float initialAngle, float initialX) {
        this->initialX = initialX;
        velocity = initialVelocity;
        angle = initialAngle;
        isActive = true;
    }

    bool updatePosition(projectile_t& projectile, float deltaTime, platform_t* platforms, uint8_t numPlatforms, float distance) {
        deltaTime = std::min(deltaTime, 0.016f);

        // Actualizar posición del proyectil
        projectile.pos.x += velocity * std::cos(angle) * deltaTime;
        projectile.pos.y += velocity * std::sin(angle) * deltaTime + 0.5f * acceleration * deltaTime * deltaTime;

        // Verificar colisiones con plataformas
        for (int i = 0; i < numPlatforms; i++) { // falta calcular offset de las plataformas 32*32 cada una
            bool horizontalOverlap = (projectile.pos.x >= platforms[i].pos.x) && 
                                   (projectile.pos.x <= platforms[i].pos.x + WIDTH_PLATFORM);
            bool verticalOverlap = (projectile.pos.y >= platforms[i].pos.y) && 
                                  (projectile.pos.y <= platforms[i].pos.y + HEIGHT_PLATFORM);

            if (horizontalOverlap && verticalOverlap) {
                // Colisión con una plataforma, detener el proyectil
                std::cout << "Colision con plataforma" << std::endl;
                return false;
            }
        }

        // Verificar si el proyectil se ha salido de los límites del nivel
        if (projectile.pos.x < 0 || projectile.pos.x > LEVEL_WIDTH || 
            projectile.pos.y < 0 || projectile.pos.y > LEVEL_HEIGHT) {
            // Proyectil fuera de los límites, desactivarlo
            std::cout << "Proyectil fuera de los límites" << std::endl;
            return false;
        }


        if (abs(projectile.pos.x - initialX) > distance*32) {
            // Proyectil fuera de los límites, desactivarlo
            std::cout << "Proyectil llego a su distancia" << std::endl;
            return false;
        }

        // El proyectil sigue activo
        return true;
    }

    bool isProjectileActive() const { return isActive; }
};

#endif // PROJECTILE_PHYSICS_H