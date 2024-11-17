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
    position_t position;  // Posición del spawn
    bool hasSomething;       // Indica si el spawn actualmente tiene un arma
    bool duckCanSpawn;      // si es para los patos.
    float respawnTimer;   // Tiempo restante para que el arma reaparezca

    Spawn(uint8_t x, uint8_t y, bool hasWeapon, bool duckCanSpawn, float respawnTimer) : position{x, y}, hasSomething(hasWeapon), duckCanSpawn(duckCanSpawn), respawnTimer(respawnTimer) {}
};


class Level {
private:
    weapon_t nullWeapon = {{0, 0}, NULL_WEAPON, 0};
    armor_t nullArmor = {{0, 0}, NULL_ARMOR};
    // Método estático que inicializa el nivel según su ID
    
    void createLevelById(int id);
    // Método que crea el nivel 0 con plataformas, cajas, etc.
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

public:
    // Constructor
    explicit Level();

    // Métodos para acceder a la información del nivel
    level_t& getLevel();
    void createNewLevel();
    void initWinningLevel(); 
    position_t getSpawnPosition();
    uint8_t getRandomWeapon();
    uint8_t getRandomArmor();
    std::vector<std::shared_ptr<Box>> getBoxes() { return boxes; }
    std::vector<std::shared_ptr<Spawn>> getSpawns() { return spawns; }
    std::unique_ptr<Box> getRandomBox(int x, int y);
    spawn_place_t getRandomSpawnPlace(int x, int y);
    void updateState(level_t& state);

    // Destructor
    ~Level() = default;
};

#endif // LEVEL_H


