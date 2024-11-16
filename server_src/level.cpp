#include "level.h"

weapon_t null_weapon = {
    {0, 0},
    NULL_WEAPON
};
armor_t null_armor = {
    {0, 0},
    NULL_ARMOR
};

Level::Level(int id) : chosenLevel(id) {
    srand(static_cast<unsigned>(time(NULL)));
    createLevelById(id);
}

// Implementación de getLevelById
void Level::createLevelById(int id) {
    switch (id) {
        case 0:
            std::cout << "Creando nivel 0\n";
            initLevel0();
        // Agregar más casos aquí para otros niveles si es necesario
        default:
            initLevel0();  // Nivel predeterminado si el ID no coincide
    }
}

// Implementación de getLevel0
void Level::initLevel0() {
    levelState.num_ducks = 0;  // Ejemplo: 4 patos en este nivel
    for (int i = 0; i < levelState.num_ducks; ++i)
        levelState.ducks[i] = {};

    // Configuración de plataformas 
    levelState.num_platforms = 30;
    for (int i = 0; i < 30; ++i) {
        levelState.platforms[i].pos = {i * WIDTH_PLATFORM, 650};
        levelState.platforms[i].type = GRASS_PLATFORM;
    }

    levelState.num_platforms += 14;
    for (int i = 30; i < 44; ++i) {
        levelState.platforms[i].pos = {i * WIDTH_PLATFORM - 22*WIDTH_PLATFORM, 550};
        levelState.platforms[i].type = GRASS_PLATFORM;
    }

    levelState.num_platforms += 14;
    for (int i = 44; i < 58; ++i) {
        // dibujo las plataformas en forma de
        if (i < 51) {
            levelState.platforms[i].pos = {i * WIDTH_PLATFORM - 44*WIDTH_PLATFORM, 450};

        } else {
            levelState.platforms[i].pos = {i * WIDTH_PLATFORM - 28*WIDTH_PLATFORM, 450};
        }
        levelState.platforms[i].type = GRASS_PLATFORM;
    }


    levelState.num_platforms += 14;
    for (int i = 58; i < 72; ++i) {
        if (i < 65) {
            levelState.platforms[i].pos = {i * WIDTH_PLATFORM - 50*WIDTH_PLATFORM, 350-i*HEIGHT_PLATFORM+58*HEIGHT_PLATFORM};
            int ini = (i+1) * WIDTH_PLATFORM - 50*WIDTH_PLATFORM;
            int fin = 64 * WIDTH_PLATFORM - 50*WIDTH_PLATFORM;
            while (ini <= fin) {
                levelState.platforms[levelState.num_platforms].pos = {ini, 350-i*HEIGHT_PLATFORM+58*HEIGHT_PLATFORM};
                levelState.platforms[levelState.num_platforms++].type = DIRT_PLATFORM;
                ini += WIDTH_PLATFORM;
            }
        } else {
            levelState.platforms[i].pos = {i * WIDTH_PLATFORM - 50*WIDTH_PLATFORM, 350+i*HEIGHT_PLATFORM-71*HEIGHT_PLATFORM};
            int ini =  65*WIDTH_PLATFORM - 50*WIDTH_PLATFORM;
            int fin = (i) * WIDTH_PLATFORM - 50*WIDTH_PLATFORM;
            while (ini < fin) {
                levelState.platforms[levelState.num_platforms].pos = {ini, 350+i*HEIGHT_PLATFORM-71*HEIGHT_PLATFORM};
                levelState.platforms[levelState.num_platforms++].type = DIRT_PLATFORM;
                ini += WIDTH_PLATFORM;
            }
        }
        levelState.platforms[i].type = GRASS_PLATFORM;
    }



    //  -----------                 -------------
    //              ---------------                
    //  -----------------------------------------
    // Configuración de lugares de aparición (spawn)
    levelState.num_spawn_places = 8;
    
    levelState.spawn_places[0].pos = {50, 500};
    levelState.spawn_places[0].is_active = true;
    levelState.spawn_places[0].weapon = null_weapon;
    levelState.spawn_places[0].armor = null_armor;
    
    levelState.spawn_places[1].pos = {100, 500};
    levelState.spawn_places[1].is_active = true;
    levelState.spawn_places[1].weapon = null_weapon;
    levelState.spawn_places[1].armor = null_armor;

    levelState.spawn_places[2].pos = {874, 500};
    levelState.spawn_places[2].is_active = true;
    levelState.spawn_places[2].weapon = null_weapon;
    levelState.spawn_places[2].armor = null_armor;

    levelState.spawn_places[3].pos = {924, 500}; // no mas de cuatro jugadores.
    levelState.spawn_places[3].is_active = true;
    levelState.spawn_places[3].weapon = null_weapon;
    levelState.spawn_places[3].armor = null_armor;

    std::cout << levelState.spawn_places[0].pos.x << " " << levelState.spawn_places[0].pos.y << std::endl;
    levelState.spawn_places[4] = getRandomSpawnPlace(75, 418);
    levelState.spawn_places[5] = getRandomSpawnPlace(425, 518);
    levelState.spawn_places[6] = getRandomSpawnPlace(510, 518);
    levelState.spawn_places[7] = getRandomSpawnPlace(875, 418);

    // Inicialización de cajas y proyectiles
    this->boxes.clear();
    this->boxes.push_back(getRandomBox(490, 152));
    this->boxes.push_back(getRandomBox(335, 642));
    this->boxes.push_back(getRandomBox(435, 642));
    this->boxes.push_back(getRandomBox(535, 642));
    this->boxes.push_back(getRandomBox(635, 642)); // 642
    
    levelState.num_boxes = 5;
    for (int i = 0; i < levelState.num_boxes; ++i)
        levelState.boxes[i] = boxes[i]->getBoxState();

    levelState.num_projectiles = 0;
    for (int i = 0; i < levelState.num_projectiles; ++i)
        levelState.projectiles[i] = {};
    
    levelState.num_dropped_weapons = 0;
    for (int i = 0; i < levelState.num_dropped_weapons; ++i)
        levelState.dropped_weapons[i] = {};

    levelState.num_dropped_armors = 0;
    for (int i = 0; i < levelState.num_dropped_armors; ++i)
        levelState.dropped_armors[i] = {};
}

// Métodos para acceder a la información del nivel
level_t& Level::getLevel() {
    return levelState;
}

position_t Level::getSpawnPosition() {
    for (int i = 0; i < levelState.num_spawn_places; ++i) {
        if (levelState.spawn_places[i].is_active) {
            levelState.spawn_places[i].is_active = false;
            return levelState.spawn_places[i].pos;
        }
    }
    return {0, 0};  // Retorna posición nula si no hay spawn activo
}

std::unique_ptr<Box> Level::getRandomBox(int x, int y) {
    int randomIndex = (rand() % MAX_BOXES)+1;
    std::cout << "random box" << std::endl;

    box_t box = { {x, y}, BOX_HEALTH, false };
    armor_t armor = null_armor;
    weapon_t weapon = null_weapon;

    if (randomIndex < MAX_BOXES / 3) {
        box.is_explosive = true;
    } else if (randomIndex < 2 * MAX_BOXES / 3) {
        armor = {{x, y}, getRandomArmor()};
    } else {
        weapon = {{x, y}, getRandomWeapon()};
    }

    return std::make_unique<Box>(box, armor, weapon);
}

uint8_t Level::getRandomWeapon() {
    std::cout << "random weapon" << std::endl;
    int randomIndex = (rand() % WEAPON_COUNT) + 1;
    std::cout << "random weapon" << std::endl;
    return randomIndex;
}

uint8_t Level::getRandomArmor() {
    std::cout << "random armor" << std::endl;
    int randomIndex = (rand() % ARMOR_COUNT)+1;
    std::cout << "random armor" << std::endl;
    return randomIndex;
}

spawn_place_t Level::getRandomSpawnPlace(int x, int y) {
    std::cout << "Random spawn" << std::endl;
    int randomIndex = (rand() % levelState.num_spawn_places)+1;
    std::cout << "Random spawn" << std::endl;

    spawn_place_t spawn = {
        {x, y},
        false,
        null_weapon,
        null_armor
    };

    if (randomIndex < levelState.num_spawn_places / 2) {
        spawn.armor.pos = {x, y};
        spawn.armor.type = getRandomArmor();
    } else {
        spawn.weapon.pos = {x, y};
        spawn.weapon.type = getRandomWeapon();
        spawn.weapon.ammo = 30;
        std::cout << "random weapon" << std::endl;
    }
    std::cout << "Random spawn" << std::endl;
    return spawn;
}

void Level::updateState(level_t& state) {
    levelState = state;
}