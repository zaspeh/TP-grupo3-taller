#ifndef LEVEL_H
#define LEVEL_H


#include "../common_src/game_state.h"
#include "../common_src/utils.h"
#include <vector>
#include <random>
#include <chrono>
#include <iostream>

class Level {
private:
    // Método estático que inicializa el nivel según su ID
    level_t getLevelById(int id);
    // Método que crea el nivel 0 con plataformas, cajas, etc.
    level_t getLevel0();
    level_t level;
    int chosenLevel;
    std::mt19937 rng;

public:
    // Constructor
    explicit Level(int id);

    // Métodos para acceder a la información del nivel
    level_t getLevel();
    position_t getSpawnPosition();
    uint8_t getRandomWeapon();
    uint8_t getRandomArmor();
    box_t getRandomBox(int x, int y);
    spawn_place_t getRandomSpawnPlace(int x, int y);

    // Destructor
    ~Level() = default;
};

#endif // LEVEL_H


