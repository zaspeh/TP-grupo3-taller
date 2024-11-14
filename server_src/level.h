#ifndef LEVEL_H
#define LEVEL_H


#include "../common_src/game_state.h"
#include "../common_src/utils.h"
#include "box.h"
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <memory>

class Level {
private:
    // Método estático que inicializa el nivel según su ID
    void createLevelById(int id);
    // Método que crea el nivel 0 con plataformas, cajas, etc.
    void initLevel0();
    level_t levelState;
    int chosenLevel;
    std::mt19937 rng;
    std::vector<std::shared_ptr<Box>> boxes;

public:
    // Constructor
    explicit Level(int id);

    // Métodos para acceder a la información del nivel
    level_t getLevel();
    position_t getSpawnPosition();
    uint8_t getRandomWeapon();
    uint8_t getRandomArmor();
    std::vector<std::shared_ptr<Box>> getBoxes() { return boxes; }
    std::unique_ptr<Box> getRandomBox(int x, int y);
    spawn_place_t getRandomSpawnPlace(int x, int y);
    void updateState(level_t state) { levelState = state; };

    // Destructor
    ~Level() = default;
};

#endif // LEVEL_H


