#ifndef LEVEL_H
#define LEVEL_H

#include "../common_src/game_state.h"
#include <vector>

class Level {
private:
    // Método estático que inicializa el nivel según su ID
    static level_t instanceLevel(int id) {
        switch (id) {
            case 0:
                return getLevel0();
            // Podrías agregar más casos aquí para otros niveles
            default:
                return getLevel0();  // Nivel predeterminado si el ID no coincide
        }
    }

    // Definición de la estructura level_t
    /*
    typedef struct {
    uint8_t num_ducks;
    duck_t ducks[MAX_DUCKS];
    uint8_t num_platforms;
    platform_t platforms[MAX_PLATFORMS];
    uint8_t num_spawn_places;
    spawn_place_t spawn_places[MAX_SPAWN_PLACES];
    uint8_t num_boxes;
    box_t boxes[MAX_BOXES];
    uint8_t num_projectiles;
    projectile_t projectiles[MAX_PROJECTILES];
    } level_t;
    */

    // Método que crea el nivel 0 con plataformas, cajas, etc.
    static level_t getLevel0() {
        level_t level;
        
        level.num_ducks = 0; // Ejemplo: 4 patos en este nivel
        for (int i = 0; i < level.num_ducks; ++i)
            level.ducks[i] = {}; 

        // Configuración de plataformas (ejemplo)
        level.num_platforms = 14;
        level.platforms[0].platform = {100, 200, 32, 32};  // Plataforma 1
        level.platforms[1].platform = {132, 200, 32, 32};  // Plataforma 2
        level.platforms[2].platform = {164, 200, 32, 32};  // Plataforma 3
        level.platforms[3].platform = {196, 200, 32, 32};  // Plataforma 4
        level.platforms[4].platform = {228, 200, 32, 32};  // Plataforma 5
        level.platforms[5].platform = {260, 200, 32, 32};  // Plataforma 6
        level.platforms[6].platform = {292, 200, 32, 32};  // Plataforma 7
        level.platforms[7].platform = {324, 200, 32, 32};  // Plataforma 8
        level.platforms[8].platform = {356, 200, 32, 32};  // Plataforma 8
        level.platforms[9].platform = {388, 200, 32, 32};  // Plataforma 8
        level.platforms[10].platform = {420, 200, 32, 32};  // Plataforma 8
        level.platforms[11].platform = {452, 200, 32, 32};  // Plataforma 8
        level.platforms[12].platform = {484, 200, 32, 32};  // Plataforma 8
        level.platforms[13].platform = {1000, 200, 32, 32};  // Plataforma 8

        // Configuración de lugares de aparición (spawn)
        level.num_spawn_places = 0;
        for (int i = 0; i < level.num_spawn_places; ++i)
            level.spawn_places[i] = {}; 

        level.num_boxes = 0;                          
        for (int i = 0; i < level.num_boxes; ++i)
            level.boxes[i] = {};
                                // boxes array (inicialización vacía)
        level.num_projectiles = 0;                          // num_projectiles
        for (int i = 0; i < level.num_projectiles; ++i)
            level.projectiles[i] = {};
                                // projectiles array (inicialización vacía)
        return level;
    }

protected:
    level_t level;

public:
    // Constructor
    explicit Level(int id) {
        level = instanceLevel(id);
    }

    level_t getLevel() const { return level; }
    
    // Destructor
    ~Level() = default;

    // Métodos para acceder a la información del nivel
    const level_t& getLevelData() const { return level; }
};

#endif // LEVEL_H