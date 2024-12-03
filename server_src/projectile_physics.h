#ifndef PROJECTILE_PHYSICS_H
#define PROJECTILE_PHYSICS_H

#include <cmath>
#include <algorithm>
#include <map>
#include <memory>
#include "../common_src/utils.h"
#include "level.h"
#include "box.h"
#include "game_state.h"
#include "player_state.h"
#include "../common_src/config_manager.h"

class ProjectilePhysics {
private:
    float velocity;
    float velocityX;    
    float velocityY;    
    float angle;
    float initialX;
    bool isActive;
    float gravity;
    YAML::Node config = ConfigManager::getInstance();
    
public:
    ProjectilePhysics();

    void initProjectile(float initialVelocity, float initialAngle, float initialX, float gravit);

    bool updatePosition(projectile_t &projectile, level_t& levelState, 
                        float deltaTime, float distance, 
                        std::map<uint8_t, std::shared_ptr<PlayerState>> &players);

    bool isProjectileActive() const;
};

#endif // PROJECTILE_PHYSICS_H
