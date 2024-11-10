#ifndef LEVEL_H
#define LEVEL_H

#include "../common_src/game_state.h"
#include <vector>

class Level {
private:
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

    // Método estático que inicializa el nivel según su ID
    static level_t getLevelById(int id) {
        switch (id) {
            case 0:
                return getLevel0();
            // Agregar más casos aquí para otros niveles si es necesario
            default:
                return getLevel0();  // Nivel predeterminado si el ID no coincide
        }
    }

    // Método que crea el nivel 0 con plataformas, cajas, etc.
    static level_t getLevel0() {
        level_t level;
        
        level.num_ducks = 0; // Ejemplo: 4 patos en este nivel
        for (int i = 0; i < level.num_ducks; ++i)
            level.ducks[i] = {}; 

        // Configuración de plataformas (ejemplo)
        level.num_platforms = 14;
        level.platforms[0].pos = {100, 200};  // Plataforma 1
        level.platforms[1].pos = {132, 200};  // Plataforma 2
        level.platforms[2].pos = {164, 200};  // Plataforma 3
        level.platforms[3].pos = {196, 200};  // Plataforma 4
        level.platforms[4].pos = {228, 200};  // Plataforma 5
        level.platforms[5].pos = {260, 200};  // Plataforma 6
        level.platforms[6].pos = {292, 200};  // Plataforma 7
        level.platforms[7].pos = {324, 200};  // Plataforma 8
        level.platforms[8].pos = {356, 200};  // Plataforma 9
        level.platforms[9].pos = {388, 200};  // Plataforma 10
        level.platforms[10].pos = {420, 200}; // Plataforma 11
        level.platforms[11].pos = {452, 200}; // Plataforma 12
        level.platforms[12].pos = {484, 200}; // Plataforma 13
        level.platforms[13].pos = {420, 160}; // Plataforma 14

        for (int i = 0; i < level.num_platforms; ++i)
            level.platforms[i].type = GRASS_PLATFORM;


        // Configuración de lugares de aparición (spawn)
        level.num_spawn_places = 2;
        level.spawn_places[0].pos = {150, 140};
        level.spawn_places[0].is_active = true;

        level.spawn_places[1].pos = {300, 140};
        level.spawn_places[1].is_active = true;

        // Inicialización de cajas y proyectiles
        level.num_boxes = 1;
        weapon_t weapon = {
            {0,0},
            0,
            0,
            false
        };
        armor_t armor = {
            {0,0},
            0,
            false
        };
        level.boxes[0] = {420, 170, false, false, weapon, armor};
        

        level.num_projectiles = 0;
        for (int i = 0; i < level.num_projectiles; ++i)
            level.projectiles[i] = {};

        return level;
    }

protected:
    level_t level;
    int chosenLevel;

public:
    // Constructor
    explicit Level(int id) : level(getLevelById(id)), chosenLevel(id) {}

    // Métodos para acceder a la información del nivel
    level_t getLevel() { return level; }

    position_t getSpawnPosition() {
        for (int i = 0; i < level.num_spawn_places; ++i) {
            if (level.spawn_places[i].is_active) {
                level.spawn_places[i].is_active = false;
                return level.spawn_places[i].pos;
            }
        }
        return {0, 0};  // Retorna posición nula si no hay spawn activo
    }

    // Destructor
    ~Level() = default;
};

#endif // LEVEL_H