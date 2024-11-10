#ifndef LEVEL_H
#define LEVEL_H


#include "../common_src/game_state.h"
#include "../common_src/utils.h"
#include <vector>

class Level {
private:
    // Método estático que inicializa el nivel según su ID
    static level_t getLevelById(int id);

    // Método que crea el nivel 0 con plataformas, cajas, etc.
    static level_t getLevel0();
    static void FillRect(level_t &level, int xIni, int xFin, int height);

protected:
    level_t level;
    int chosenLevel;

public:
    // Constructor
    explicit Level(int id);

    // Métodos para acceder a la información del nivel
    level_t getLevel();
    position_t getSpawnPosition();

    // Destructor
    ~Level() = default;
};

#endif // LEVEL_H


