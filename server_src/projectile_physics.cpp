#include "projectile_physics.h"
#include "game_state.h"
/*
ProjectilePhysics::ProjectilePhysics() :
    velocity(0.0f),
    acceleration(0.0f),
    angle(0.0f),
    isActive(false) {}

void ProjectilePhysics::initProjectile(float initialVelocity, float initialAngle, float initialX) {
    this->initialX = initialX;
    velocity = initialVelocity;
    angle = initialAngle;
    isActive = true;
}

bool ProjectilePhysics::updatePosition(projectile_t &projectile, level_t& levelState, 
                                     float deltaTime, float distance, GameState* gameState) {
    if (projectile.is_active == false) {
        return false;
    }
    deltaTime = std::min(deltaTime, 0.016f);

    // Actualizar posición del proyectil
    projectile.pos.x += velocity * std::cos(angle) * deltaTime;
    projectile.pos.y += velocity * std::sin(angle) * deltaTime + 
                       0.5f * acceleration * deltaTime * deltaTime;

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
    auto players = gameState->getPlayers();
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

bool ProjectilePhysics::isProjectileActive() const { 
    return isActive; 
}
*/