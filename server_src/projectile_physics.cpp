#include "projectile_physics.h"
#include "game_state.h"

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

bool ProjectilePhysics::updatePosition(projectile_t &projectile, Level* level, 
                                     float deltaTime, float distance, GameState* gameState) {
    if (projectile.is_active == false) {
        return false;
    }
    deltaTime = std::min(deltaTime, 0.016f);

    // Actualizar posición del proyectil
    projectile.pos.x += velocity * std::cos(angle) * deltaTime;
    projectile.pos.y += velocity * std::sin(angle) * deltaTime + 
                       0.5f * acceleration * deltaTime * deltaTime;

    // Verificar colisiones con plataformas
    level_t& levelState = level->getLevel();
    for (int i = 0; i < levelState.num_platforms; i++) {
        bool horizontalOverlap = (projectile.pos.x >= levelState.platforms[i].pos.x) && 
                               (projectile.pos.x <= levelState.platforms[i].pos.x + WIDTH_PLATFORM);
        bool verticalOverlap = (projectile.pos.y >= levelState.platforms[i].pos.y) && 
                              (projectile.pos.y <= levelState.platforms[i].pos.y + HEIGHT_PLATFORM);

        if (horizontalOverlap && verticalOverlap) {
            return false;
        }
    }

    for (int i = 0; i < levelState.num_boxes; i++) {
        if(levelState.boxes[i].health <= 0) continue;
        
        float projectileRadius = 5.0f;
        float boxWidth = 32.0f;
        float boxHeight = 40.0f;
        
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
            std::cout << "Colisión detectada con caja " << i << std::endl;
            levelState.boxes[i].health--;
            auto boxes = level->getBoxes();
            if (boxes[i])
                boxes[i]->setBoxState(levelState.boxes[i]);
            // Si la caja se rompió, verificar si tiene un arma para soltar
            if (levelState.boxes[i].health == 0) {
                std::cout << "Caja rota, verificando contenido\n";
                if (i < int(boxes.size())) {
                    weapon_t weaponState = boxes[i]->getWeaponState();
                    boxes[i]->setBoxState(levelState.boxes[i]);
                    if (weaponState.type != NULL_WEAPON) {
                        std::cout << "Droppeando arma\n";
                        gameState->checkIfDropWeapon(weaponState);
                    }
                }
            }
            
            // Actualizar el estado del nivel después de modificar la caja
            level->updateState(levelState);
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

    return true;
}

bool ProjectilePhysics::isProjectileActive() const { 
    return isActive; 
}