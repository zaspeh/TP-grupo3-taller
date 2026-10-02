#ifndef ARMOR_PHYSICS_H
#define ARMOR_PHYSICS_H

#include "../common_src/utils.h"

class ArmorPhysics {
private:
    float verticalVelocity;
    float gravity;
    bool isOnGround;

public:
    ArmorPhysics() : verticalVelocity(0.0f), gravity(980.0f), isOnGround(false) {}

    bool updatePosition(armor_t& armor, float deltaTime, platform_t* platforms, uint8_t numPlatforms) {
        deltaTime = std::min(deltaTime, 0.016f);
        verticalVelocity += gravity * deltaTime;
        float newY = armor.pos.y + (verticalVelocity * deltaTime);

        isOnGround = false;
        for (int i = 0; i < numPlatforms; i++) {
            bool horizontalOverlap = (armor.pos.x + 20 > platforms[i].pos.x) &&
                                     (armor.pos.x < platforms[i].pos.x + WIDTH_PLATFORM);

            if (horizontalOverlap && newY + 20 > platforms[i].pos.y && armor.pos.y + 20 <= platforms[i].pos.y + 5) {
                armor.pos.y = platforms[i].pos.y - 20;
                verticalVelocity = 0;
                isOnGround = true;
                return true;
            }
        }

        if (!isOnGround) {
            armor.pos.y = newY;
        }

        return false;
    }
};

#endif // ARMOR_PHYSICS_H
