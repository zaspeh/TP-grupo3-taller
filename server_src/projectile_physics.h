#ifndef PROJECTILE_PHYSICS_H
#define PROJECTILE_PHYSICS_H

#include <cmath>
#include <algorithm>
#include "../common_src/utils.h"
#include "level.h"
#include "box.h"
#include "game_state.h"

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
                       float deltaTime, float distance, std::map<uint8_t, std::shared_ptr<PlayerState>> &players) {
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

        // colisiones con plataformas.
        for (int i = 0; i < levelState.num_platforms; i++) {
            bool horizontalOverlap = (projectile.pos.x >= levelState.platforms[i].pos.x) && 
                                (projectile.pos.x <= levelState.platforms[i].pos.x + WIDTH_PLATFORM);
            bool verticalOverlap = (projectile.pos.y >= levelState.platforms[i].pos.y) && 
                                (projectile.pos.y <= levelState.platforms[i].pos.y + HEIGHT_PLATFORM);

            if (horizontalOverlap && verticalOverlap) {
                return false;
            }
        }

        // colision con cajas.
        for (int i = 0; i < levelState.num_boxes; i++) {
            if(levelState.boxes[i].health <= 0) continue;
            
            float projectileRadius = 5.0f;
            float boxWidth = 32.0f;
            float boxHeight = 40.0f; // usar las constantes
            
            float boxCenterX = levelState.boxes[i].pos.x;
            float boxCenterY = levelState.boxes[i].pos.y;
            float boxLeft = boxCenterX - (boxWidth/2);
            float boxRight = boxCenterX + (boxWidth/2);
            float boxTop = boxCenterY - (boxHeight/2);
            float boxBottom = boxCenterY + (boxHeight/2);

            bool horizontalOverlap = (projectile.pos.x + projectileRadius >= boxLeft) && 
                                    (projectile.pos.x - projectileRadius <= boxRight);
            bool verticalOverlap = (projectile.pos.y + projectileRadius >= boxTop) && 
                                    (projectile.pos.y - projectileRadius <= boxBottom);

            if (horizontalOverlap && verticalOverlap) {
                levelState.boxes[i].health--;
                return false;
            }
        }

        // Verificar si el proyectil se ha salido de los límites del nivel
        if (projectile.pos.x < 0 || projectile.pos.x > LEVEL_WIDTH || 
            projectile.pos.y < 0 || projectile.pos.y > LEVEL_HEIGHT) {
            return false;
        }

        if (abs(projectile.pos.x - initialX) > distance*32) {
            return false;
        }

        // colision con jugadores
        for (int i = 0; i < levelState.num_ducks; i++) {
            std::cout << "Colision con jugador: " << i << std::endl;
            if (levelState.ducks[i].isAlive == false || !players[i]->isAlive()) continue;
            
            float duckCenterX = levelState.ducks[i].pos.x;
            float duckCenterY = levelState.ducks[i].pos.y;
            float duckLeft = duckCenterX - (WIDTH_DUCK/2);
            float duckRight = duckCenterX + (WIDTH_DUCK/2);
            float duckTop = duckCenterY - (HEIGHT_DUCK/2);
            float duckBottom = duckCenterY + (HEIGHT_DUCK/2);

            bool horizontalOverlap = (projectile.pos.x + PROJECTILE_RADIUS >= duckLeft) && 
                                    (projectile.pos.x - PROJECTILE_RADIUS <= duckRight);
            bool verticalOverlap = (projectile.pos.y + PROJECTILE_RADIUS >= duckTop) && 
                                    (projectile.pos.y - PROJECTILE_RADIUS <= duckBottom);

            if (horizontalOverlap && verticalOverlap) {


                if (levelState.ducks[i].helmet.type != NULL_ARMOR) { // 1ro le saco el casco
                    levelState.ducks[i].helmet.type = NULL_ARMOR;
                    players[i]->setHelmetEquipped(levelState.ducks[i].helmet);
                } else if (levelState.ducks[i].chestplate.type != NULL_ARMOR) { // 2do le saco la pechera
                    levelState.ducks[i].chestplate.type = NULL_ARMOR;
                    players[i]->setArmorEquipped(levelState.ducks[i].chestplate);
                } else { // 3ro lo mato
                    levelState.ducks[i].isAlive = false;
                    players[i]->setAlive();
                    std::cout << "jugador " << i << " muertoOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO" << std::endl;
                }

                return false;
            }
        }



        return true;
    }

    bool isProjectileActive() const { 
        return isActive; 
    }
};

#endif // PROJECTILE_PHYSICS_H

