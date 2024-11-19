#ifndef LEVEL_H
#define LEVEL_H


#include "../common_src/game_state.h"
#include "../common_src/utils.h"
#include "box.h"
#include "weapon.h"
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <memory>

struct Spawn {
    position_t position;  
    bool hasSomething;       
    bool duckCanSpawn;      
    float respawnTimer;  

    Spawn(uint8_t x, uint8_t y, bool hasWeapon, bool duckCanSpawn, float respawnTimer) : position{x, y}, hasSomething(hasWeapon), duckCanSpawn(duckCanSpawn), respawnTimer(respawnTimer) {}
};


class Level {
private:
    weapon_t nullWeapon = {{0, 0}, NULL_WEAPON, 0};
    armor_t nullArmor = {{0, 0}, NULL_ARMOR};
    
    void createLevelById(int id);
    void clearLevelState();
    void initLevel0();
    void initLevel1();
    void initLevel2();
    void winningLevel();
    level_t levelState;
    int chosenLevel;
    std::mt19937 rng;
    std::uniform_int_distribution<int> boxDist;
    std::uniform_int_distribution<int> weaponDist;
    std::uniform_int_distribution<int> armorDist;
    std::vector<std::shared_ptr<Box>> boxes;
    std::vector<std::shared_ptr<Spawn>> spawns;

    std::unique_ptr<Box> getRandomBox(int x, int y);
    void updateState(level_t& state);
    uint8_t getRandomWeapon();
    uint8_t getRandomArmor();

public:

    explicit Level();

    level_t& getLevel();
    void createNewLevel();
    void initWinningLevel(); 
    position_t getSpawnPosition();
    std::vector<std::shared_ptr<Box>> getBoxes() { return boxes; }
    std::vector<std::shared_ptr<Spawn>> getSpawns() { return spawns; }
    spawn_place_t getRandomSpawnPlace(int x, int y);
    ~Level() = default;
};

#endif // LEVEL_H


