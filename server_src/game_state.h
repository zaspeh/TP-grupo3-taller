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

#define WHITE 1
#define YELLOW 2
#define GRAY 3
#define RED 4

class Server;
class ProjectilePhysics;

class GameState {
private:
    game_state_t state;
    std::map<uint8_t, std::shared_ptr<PlayerState>> players;
    Level level;
    level_t currentLevel;
    mutable std::mutex mtx;
    bool pickAnyWeapon = false;
    bool matchFinished = false;
    std::vector<WeaponPhysics> fallingWeapons;
    std::vector<weapon_t> weaponsInAir;
    std::vector<ArmorPhysics> fallingArmors; 
    std::vector<armor_t> armorsInAir;  
    std::vector<ProjectilePhysics> projectilePhysics;
    std::atomic<bool>& gameShouldContinue;

    bool isColor(uint8_t action);
    void checkIfSomeoneWin();
    void finishMatch(uint8_t id);
    void changeLevel();
    bool explotionInPosition(position_t position);
    void checkIfDropArmor(armor_t droppedArmor);
    void updateArmorsPhysics(float deltaTime);
    void updateBoxes();
    void updateSpawns(float deltaTime);
    uint8_t checkWeaponDistance(uint8_t weaponType);
    bool chosedAWeapon(uint8_t id, uint8_t action);
    void checkIfDropWeapon(weapon_t droppedWeapon);
    void updateWeaponsPhysics(float deltaTime);
    void checkProjectils(std::shared_ptr<PlayerState> player);
    void updateProjectilsPhysics(float deltaTime);
    void createFiveShoots(int x, int y);
    void createProjectile(uint8_t weaponType, position_t origin, bool facingLeft, float randomAngle = 0.0f);
    float getRandomAngle(bool faceLefting);
    armor_t getArmorPosition(position_t position, bool helmetEquipped, bool armorEquipped);
    weapon_t getWeaponPosition(position_t position);
    std::shared_ptr<Weapon> createWeapon(weapon_t weapon);
    std::map<uint8_t, std::shared_ptr<PlayerState>> getPlayers();
    std::shared_ptr<PlayerState> getPlayer(uint8_t id);
    void updateState(uint8_t id, std::shared_ptr<PlayerState> player);
    std::shared_ptr<PlayerState> connectPlayer(uint8_t id);
    void compactDucks(); 

public:
    GameState(std::atomic<bool>& gameShouldContinue);

    void removePlayer(uint8_t id);
    game_state_t doAction(uint8_t id, uint8_t action);
    game_state_t updatePlayers(float deltaTime);
};

#endif // GAME_STATE_H
