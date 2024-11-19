#include "player_state.h"

PlayerState::PlayerState(uint8_t clientID, int x, int y) : 
    weapon(nullptr), 
    verticalVelocity(0.0)
{
    duck.pos = {x, y};
    duck.id = clientID;
    duck.faceLeft = false;
    duck.isJumping = false;
    duck.isDucking = false;
    duck.isFalling = false;  
    duck.isFlaping = false;  
    duck.health = 1;
    duck.isAlive = true;
    duck.score = 0;
    duck.color = 0;
    duck.equipped_weapon = nullWeapon;
    duck.helmet = nullArmor;
    duck.chestplate = nullArmor;
    infinitAmmo = false;
}

void PlayerState::resetPlayer(duck_t newDuck, int x , int y) {
    duck = newDuck;
    duck.pos = {x, y};
    duck.isAlive = true;
    duck.isJumping = false;
    duck.isFalling = false;   
    duck.isFlaping = false;
    duck.equipped_weapon.type = NULL_WEAPON;
    duck.chestplate.type = NULL_ARMOR;
    duck.helmet.type = NULL_ARMOR;
}

bool PlayerState::doNotCollideX(platform_t* plat, uint8_t numPlats, int new_x) {
    for (int i = 0; i < numPlats; i++) {
        if (duck.pos.y + HEIGHT_DUCK > plat[i].pos.y && 
            duck.pos.y < plat[i].pos.y + HEIGHT_PLATFORM) {

            if (new_x > plat[i].pos.x && 
                new_x < plat[i].pos.x + WIDTH_PLATFORM) {
                return false;
            }
        }
    }
    return true;
}

void PlayerState::move(int dx, int dy, platform_t* plat, uint8_t numPlats) {
    if (dx != 0) {
        int newX = duck.pos.x + dx;
        if (doNotCollideX(plat, numPlats, newX)) {
            duck.pos.x = newX;
        }
    }
}

void PlayerState::updateWeapon(float deltaTime, level_t& level) {
    if (weapon != nullptr && weapon->getAmmo() == 0 && (weapon->getType() == GRENADE_WEAPON || weapon->getType() == BANANA_WEAPON)) {
        duck.equipped_weapon.type = NULL_WEAPON;
    }

    if (weapon != nullptr && weapon->getType() == GRENADE_WEAPON) {
        Grenade* grenade = dynamic_cast<Grenade*>(weapon); // Downcasting
        if (grenade->getTimeToExplode() <= 0){
            level.explosions[level.num_explosions++] = getPosition();
            duck.equipped_weapon.type = NULL_WEAPON;
            weapon = nullptr;
        }
        if (grenade->getPinPulled()) {
            grenade->setTimeToExplode(grenade->getTimeToExplode() - deltaTime);
        }
    }
}

void PlayerState::updatePosition(float deltaTime, platform_t* platforms, uint8_t numPlatforms, position_t* explotions, uint8_t numExplotions) {
    deltaTime = std::min(deltaTime, 0.016f);

    duck.equipped_weapon.pos = duck.pos;
    duck.helmet.pos = duck.pos;
    duck.chestplate.pos = duck.pos;

    verticalVelocity += gravity * deltaTime;
    float newY = duck.pos.y + (verticalVelocity * deltaTime);
    
    isOnGround = false;
    bool hitCeiling = false;
    
    for (int i = 0; i < numPlatforms; i++) {
        bool horizontalOverlap = (duck.pos.x + WIDTH_DUCK > platforms[i].pos.x + 20) && 
                                (duck.pos.x < platforms[i].pos.x + WIDTH_PLATFORM); 
        
        if (horizontalOverlap) {
            if (newY + HEIGHT_DUCK > platforms[i].pos.y && 
                duck.pos.y + HEIGHT_DUCK <= platforms[i].pos.y + 10) { 
                duck.pos.y = platforms[i].pos.y - HEIGHT_DUCK;
                verticalVelocity = 0;
                isOnGround = true;
                duck.isJumping = false;
                duck.isFalling = false;
                duck.isFlaping = false;
                break;
            }
            else if (newY < platforms[i].pos.y + HEIGHT_PLATFORM && 
                     duck.pos.y >= platforms[i].pos.y + HEIGHT_PLATFORM - 5) { 
                if (duck.isJumping || verticalVelocity < 0) {
                    duck.pos.y = platforms[i].pos.y + HEIGHT_PLATFORM;
                    verticalVelocity = 0;
                    hitCeiling = true;
                    duck.isJumping = false;
                    duck.isFalling = true;
                }
                break;
            }
        }
    }

    for (int i = 0; i < numExplotions; i++) {
        bool horizontalOverlap = (duck.pos.x + WIDTH_DUCK > explotions[i].x) &&
                                 (duck.pos.x < explotions[i].x + WIDTH_EXPLOTION);
        bool verticalOverlap = (duck.pos.y + HEIGHT_DUCK > explotions[i].y) &&
                                (duck.pos.y < explotions[i].y + HEIGHT_EXPLOTION);
        if (horizontalOverlap && verticalOverlap) {
            duck.isAlive = false;
        }
    }

    if (!isOnGround && !hitCeiling) {
        duck.pos.y = newY;
        duck.isFalling = verticalVelocity > 0;
    }

    float maxFallSpeed = 800.0f;
    if (duck.isFlaping) {
        maxFallSpeed = 300.0f;
    }
    if (verticalVelocity > maxFallSpeed) {
        verticalVelocity = maxFallSpeed;
    }

    if(duck.pos.y > 1024)   
        duck.isAlive = false;
}

void PlayerState::jump() {
    if (isOnGround && !duck.isFlaping) { 
        duck.isJumping = true;
        duck.isFalling = false;
        verticalVelocity = jumpStrength;
        isOnGround = false;  
    } else if (!isOnGround){
        duck.isFlaping = !duck.isFlaping;
    }
}

void PlayerState::takeDamage(uint8_t damage) {
    if (armor.isEquipped()) {
        armor.absorb_hit();
        return;
    }
    if (helmet.isEquipped()) {
        helmet.absorb_hit();
        return;
    }
    
    duck.health -= damage;
    if (duck.health <= 0) {
        setAlive();
    }
}

void PlayerState::setArmorEquipped(armor_t armr) { 
    duck.chestplate = armr;
    armor.unequip();
    if (duck.chestplate.type != NULL_ARMOR)
        armor.equip(); 
}    

void PlayerState::setHelmetEquipped(armor_t hmt) { 
    duck.helmet = hmt;
    helmet.unequip();
    if (duck.helmet.type != NULL_ARMOR)
        helmet.equip(); 
}

weapon_t PlayerState::pickWeapon(Weapon* newWeapon) {
    weapon_t weaponST = {{0, 0},
NULL_WEAPON
    };
    if (weapon) {
        weapon_t pickedWeapon = dropWeapon();
        weaponST = pickedWeapon;
    }
    weapon = newWeapon;
    duck.equipped_weapon.type = newWeapon->getId();
    return weaponST;
}

weapon_t PlayerState::dropWeapon() {
    weapon_t weaponST = {
        {0, 0},
        NULL_WEAPON,
        0
    };
    if (weapon != nullptr) {
        weaponST = duck.equipped_weapon;
        weaponST.ammo = weapon->getAmmo();  
        weapon = nullptr;
        duck.equipped_weapon.type = NULL_WEAPON;
    }
    return weaponST;
}

bool PlayerState::shoot() {
    bool returnValue = false;

    if (weapon != nullptr) {
        if (weapon->getType() == GRENADE_WEAPON) {
            Grenade* grenade = dynamic_cast<Grenade*>(weapon); 
            if (grenade != nullptr && grenade->getPinPulled()) {
                grenade->throw_grenade();
                return true;
            } else if (!grenade->getPinPulled()) {
                weapon->shoot(infinitAmmo);
                return false;
            }
        }

        returnValue = weapon->shoot(infinitAmmo); 
    }
    
    return returnValue;
}