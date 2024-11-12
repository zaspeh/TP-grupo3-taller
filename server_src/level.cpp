#include "level.h"

// Implementación de getLevelById
level_t Level::getLevelById(int id) {
    switch (id) {
        case 0:
            return getLevel0();
        // Agregar más casos aquí para otros niveles si es necesario
        default:
            return getLevel0();  // Nivel predeterminado si el ID no coincide
    }
}

// Implementación de getLevel0
level_t Level::getLevel0() {
    level_t level;

    level.num_ducks = 0;  // Ejemplo: 4 patos en este nivel
    for (int i = 0; i < level.num_ducks; ++i)
        level.ducks[i] = {};

    // Configuración de plataformas (ejemplo)
    level.num_platforms = 30;
    for (int i = 0; i < 30; ++i) {
        level.platforms[i].pos = {i * WIDTH_PLATFORM, 650};
        level.platforms[i].type = GRASS_PLATFORM;
    }

    level.num_platforms += 14;
    for (int i = 30; i < 44; ++i) {
        level.platforms[i].pos = {i * WIDTH_PLATFORM - 22*WIDTH_PLATFORM, 550};
        level.platforms[i].type = GRASS_PLATFORM;
    }

    level.num_platforms += 14;
    for (int i = 44; i < 58; ++i) {
        // dibujo las plataformas en forma de
        if (i < 51) {
            level.platforms[i].pos = {i * WIDTH_PLATFORM - 44*WIDTH_PLATFORM, 450};

        } else {
            level.platforms[i].pos = {i * WIDTH_PLATFORM - 28*WIDTH_PLATFORM, 450};
        }
        level.platforms[i].type = GRASS_PLATFORM;
    }


    level.num_platforms += 14;
    for (int i = 58; i < 72; ++i) {
        if (i < 65) {
            level.platforms[i].pos = {i * WIDTH_PLATFORM - 50*WIDTH_PLATFORM, 350-i*HEIGHT_PLATFORM+58*HEIGHT_PLATFORM};
            int ini = (i+1) * WIDTH_PLATFORM - 50*WIDTH_PLATFORM;
            int fin = 64 * WIDTH_PLATFORM - 50*WIDTH_PLATFORM;
            while (ini <= fin) {
                level.platforms[level.num_platforms].pos = {ini, 350-i*HEIGHT_PLATFORM+58*HEIGHT_PLATFORM};
                level.platforms[level.num_platforms++].type = DIRT_PLATFORM;
                ini += WIDTH_PLATFORM;
            }
        } else {
            level.platforms[i].pos = {i * WIDTH_PLATFORM - 50*WIDTH_PLATFORM, 350+i*HEIGHT_PLATFORM-71*HEIGHT_PLATFORM};
            int ini =  65*WIDTH_PLATFORM - 50*WIDTH_PLATFORM;
            int fin = (i) * WIDTH_PLATFORM - 50*WIDTH_PLATFORM;
            while (ini < fin) {
                level.platforms[level.num_platforms].pos = {ini, 350+i*HEIGHT_PLATFORM-71*HEIGHT_PLATFORM};
                level.platforms[level.num_platforms++].type = DIRT_PLATFORM;
                ini += WIDTH_PLATFORM;
            }
        }
        level.platforms[i].type = GRASS_PLATFORM;
    }



    //  -----------                 -------------
    //              ---------------                
    //  -----------------------------------------
    // Configuración de lugares de aparición (spawn)
    level.num_spawn_places = 8;
    weapon_t null_weapon = {
        {0, 0},
        NULL_WEAPON
    };
    armor_t null_armor = {
        {0, 0},
        NULL_ARMOR
    };

    
    level.spawn_places[0].pos = {50, 500};
    level.spawn_places[0].is_active = true;
    level.spawn_places[0].weapon = null_weapon;
    level.spawn_places[0].armor = null_armor;
    
    level.spawn_places[1].pos = {100, 500};
    level.spawn_places[1].is_active = true;
    level.spawn_places[1].weapon = null_weapon;
    level.spawn_places[1].armor = null_armor;

    level.spawn_places[2].pos = {874, 500};
    level.spawn_places[2].is_active = true;
    level.spawn_places[2].weapon = null_weapon;
    level.spawn_places[2].armor = null_armor;

    level.spawn_places[3].pos = {924, 500}; // no mas de cuatro jugadores.
    level.spawn_places[3].is_active = true;
    level.spawn_places[3].weapon = null_weapon;
    level.spawn_places[3].armor = null_armor;

    // Spawpoints para armas
    level.spawn_places[4].pos = {75, 418};
    level.spawn_places[4].is_active = false;
    weapon_t weapon1 = {
        {75, 418},
        AK_47_WEAPON
    };
    level.spawn_places[4].weapon = weapon1;
    level.spawn_places[4].armor = null_armor;

        // Spawpoints para armas
    level.spawn_places[5].pos = {425, 518};
    level.spawn_places[5].is_active = false;
    weapon_t weapon2 = {
        {425, 518},
        DARTGUN_WEAPON
    };
    level.spawn_places[5].weapon = weapon2;
    level.spawn_places[5].armor = null_armor;

        // Spawpoints para armas
    level.spawn_places[6].pos = {510, 518};
    level.spawn_places[6].is_active = false;
    level.spawn_places[6].weapon = null_weapon;
    armor_t armor1 = {
        {510, 518},
        HELMET_ARMOR,
    };
    level.spawn_places[6].armor = armor1;
    
        // Spawpoints para armas
    level.spawn_places[7].pos = {875, 418};
    level.spawn_places[7].is_active = false;
    level.spawn_places[7].weapon = null_weapon;
    armor_t armor2 = {
        {875, 418},
        CHESTPLATE_ARMOR,
    };
    level.spawn_places[7].armor = armor2;
    

    // Inicialización de cajas y proyectiles
    level.num_boxes = 1;

    level.boxes[0] = {480, 518, false, false, null_weapon, null_armor};

    level.num_projectiles = 0;
    for (int i = 0; i < level.num_projectiles; ++i)
        level.projectiles[i] = {};
    

    level.num_dropped_weapons = 0;
    for (int i = 0; i < level.num_projectiles; ++i)
        level.dropped_weapons[i] = {};

    return level;
}

// Constructor
Level::Level(int id) : level(getLevelById(id)), chosenLevel(id) {}

// Métodos para acceder a la información del nivel
level_t Level::getLevel() {
    return level;
}

position_t Level::getSpawnPosition() {
    for (int i = 0; i < level.num_spawn_places; ++i) {
        if (level.spawn_places[i].is_active) {
            level.spawn_places[i].is_active = false;
            return level.spawn_places[i].pos;
        }
    }
    return {0, 0};  // Retorna posición nula si no hay spawn activo
}
