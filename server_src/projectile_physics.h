#ifndef PROJECTILE_PHYSICS_H
#define PROJECTILE_PHYSICS_H

#include <cmath>
#include <algorithm>
#include "../common_src/utils.h"
#include "level.h"

// Forward declarations
class GameState;

class ProjectilePhysics {
private:
    float initialX;
    float velocity;
    float acceleration;
    float angle;
    bool isActive;

public:
    ProjectilePhysics();
    
    void initProjectile(float initialVelocity, float initialAngle, float initialX);
    
    bool updatePosition(projectile_t &projectile, Level& level, float deltaTime, 
                       float distance, GameState* gameState);

    bool isProjectileActive() const;
};

#endif // PROJECTILE_PHYSICS_H