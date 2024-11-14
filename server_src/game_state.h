#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include "player_state.h"
#include "level.h"
#include "weapon_physics.h"
#include "armor_physics.h"
#include "projectile_physics.h"
#include "../common_src/utils.h"

class GameState {
private:
    game_state_t state;
    std::map<uint8_t, std::shared_ptr<PlayerState>> players;
    Level level;
    level_t currentLevel;
    mutable std::mutex mtx;
    std::vector<WeaponPhysics> fallingWeapons;
    std::vector<weapon_t> weaponsInAir;
    std::vector<ArmorPhysics> fallingArmors; 
    std::vector<armor_t> armorsInAir;  
    std::vector<ProjectilePhysics> projectilePhysics;

public:
    // Constructor
    GameState();

    std::shared_ptr<PlayerState> getPlayer(uint8_t id);
    void removePlayer(uint8_t id);
    void updateState(uint8_t id, std::shared_ptr<PlayerState> player);
    std::shared_ptr<PlayerState> connectPlayer(uint8_t id);
    std::map<uint8_t, std::shared_ptr<PlayerState>> getPlayers();
    game_state_t doAction(uint8_t id, uint8_t action);
    game_state_t updatePlayers(float deltaTime);
    armor_t getArmorPosition(position_t position, bool helmetEquipped, bool armorEquipped);
    weapon_t getWeaponPosition(position_t position);
    Weapon* createWeapon(uint8_t weaponType);
    void checkIfDropWeapon(weapon_t droppedWeapon);
    void updateWeaponsPhysics(float deltaTime);
    void checkProjectils(std::shared_ptr<PlayerState> player);
    void updateProjectilsPhysics(float deltaTime);
    void createProjectile(uint8_t weaponType, position_t origin, bool facingLeft);
    uint8_t checkWeaponDistance(uint8_t weaponType);
    void checkIfDropArmor(armor_t droppedArmor);
    void updateArmorsPhysics(float deltaTime);
    void updateBoxes();
};

#endif // GAME_STATE_H
