#include "projectile_physics.h"
#include <iostream>

ProjectilePhysics::ProjectilePhysics() :
    velocity(0.0f),
    velocityX(0.0f),
    velocityY(0.0f),
    angle(0.0f),
    initialX(0.0f),
    isActive(false),
    gravity(980.0f) {}

void ProjectilePhysics::initProjectile(float initialVelocity, float initialAngle, float initialX, float gravit) {
    this->initialX = initialX;
    this->velocity = initialVelocity;
    this->angle = initialAngle;
    this->gravity = gravit;
    this->velocityX = initialVelocity * std::cos(angle);
    this->velocityY = initialVelocity * std::sin(angle);
    isActive = true;
}

bool ProjectilePhysics::updatePosition(projectile_t &projectile, level_t& levelState, 
                                       float deltaTime, float distance, 
                                       std::map<uint8_t, std::shared_ptr<PlayerState>> &players) {
    if (!projectile.is_active) {
        return false;
    }

    deltaTime = std::min(deltaTime, 0.016f);

    if (projectile.type == GRENADE_WEAPON || projectile.type == BANANA_WEAPON) {
        velocityY += gravity * deltaTime;
        projectile.pos.x += velocityX * deltaTime;
        projectile.pos.y += velocityY * deltaTime;
    } else {
        projectile.pos.x += velocity * std::cos(angle) * deltaTime;
        projectile.pos.y += velocity * std::sin(angle) * deltaTime;
    }

    for (int i = 0; i < levelState.num_platforms; i++) {
        bool horizontalOverlap = (projectile.pos.x >= levelState.platforms[i].pos.x) && 
                                 (projectile.pos.x <= levelState.platforms[i].pos.x + WIDTH_PLATFORM);
        bool verticalOverlap = (projectile.pos.y >= levelState.platforms[i].pos.y) && 
                               (projectile.pos.y <= levelState.platforms[i].pos.y + HEIGHT_PLATFORM);

        if (horizontalOverlap && verticalOverlap) {
            if (projectile.type == GRENADE_WEAPON) {
                projectile.pos.y += (velocityY > 0) ? -32 : 0;
            } else if (projectile.type == BANANA_WEAPON) {
                levelState.bananas[levelState.num_bananas++] = projectile.pos;
                levelState.bananas[levelState.num_bananas].y = levelState.platforms[i].pos.y - 5;
            }
            return false;
        }
    }

    for (int i = 0; i < levelState.num_boxes; i++) {
        if(levelState.boxes[i].health <= 0) continue;

        float projectileRadius = 5.0f;
        float boxWidth = 32.0f;
        float boxHeight = 40.0f;
        float boxLeft = levelState.boxes[i].pos.x - (boxWidth / 2);
        float boxRight = levelState.boxes[i].pos.x + (boxWidth / 2);
        float boxTop = levelState.boxes[i].pos.y - (boxHeight / 2);
        float boxBottom = levelState.boxes[i].pos.y + (boxHeight / 2);

        bool horizontalOverlap = (projectile.pos.x + projectileRadius >= boxLeft) && 
                                 (projectile.pos.x - projectileRadius <= boxRight);
        bool verticalOverlap = (projectile.pos.y + projectileRadius >= boxTop) && 
                               (projectile.pos.y - projectileRadius <= boxBottom);

        if (horizontalOverlap && verticalOverlap) {
            if (projectile.type == GRENADE_WEAPON)
                projectile.pos.y += (velocityY > 0) ? -32 : 0;
            levelState.boxes[i].health--;
            return false;
        }
    }

    if (projectile.pos.x < 0 || projectile.pos.x > LEVEL_WIDTH || 
        projectile.pos.y < 0 || projectile.pos.y > LEVEL_HEIGHT) {
        return false;
    }

    if (abs(projectile.pos.x - initialX) > distance * 32) {
        return false;
    }

    for (int i = 0; i < levelState.num_ducks; i++) {
        if (!players[i]) continue;
        if (!players[i]->isAlive()) continue;
        if (projectile.type == BANANA_WEAPON) continue;

        float duckLeft = levelState.ducks[i].pos.x - (WIDTH_DUCK / 2);
        float duckRight = levelState.ducks[i].pos.x + (WIDTH_DUCK / 2);
        float duckTop = levelState.ducks[i].pos.y - (HEIGHT_DUCK / 2);
        float duckBottom = levelState.ducks[i].pos.y + (HEIGHT_DUCK / 2);

        bool horizontalOverlap = (projectile.pos.x + PROJECTILE_RADIUS >= duckLeft) && 
                                 (projectile.pos.x - PROJECTILE_RADIUS <= duckRight);
        bool verticalOverlap = (projectile.pos.y + PROJECTILE_RADIUS >= duckTop) && 
                               (projectile.pos.y - PROJECTILE_RADIUS <= duckBottom);

        if (horizontalOverlap && verticalOverlap) {
            if (levelState.ducks[i].helmet.type != NULL_ARMOR) {
                levelState.ducks[i].helmet.type = NULL_ARMOR;
                players[i]->setHelmetEquipped(levelState.ducks[i].helmet);
            } else if (levelState.ducks[i].chestplate.type != NULL_ARMOR) {
                levelState.ducks[i].chestplate.type = NULL_ARMOR;
                players[i]->setArmorEquipped(levelState.ducks[i].chestplate);
            } else {
                std::cout << "Pato debería morir\n";
                levelState.ducks[i].isAlive = false;
                players[i]->setAlive();
            }
            return false;
        }
    }

    return true;
}

bool ProjectilePhysics::isProjectileActive() const {
    return isActive;
}
    