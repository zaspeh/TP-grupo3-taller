#include "level.h"

weapon_t null_weapon = {
    {0, 0},
    NULL_WEAPON
};
armor_t null_armor = {
    {0, 0},
    NULL_ARMOR
};

Level::Level()  {
    srand(time(NULL));
    // hago un random id:
    int id = rand() % 3;
    chosenLevel = id;
    createLevelById(id);
}

void Level::createNewLevel(){
    int id = chosenLevel;
    while (id != chosenLevel) {
        id = rand() % 3;
    }
    chosenLevel = id;
    createLevelById(id);
}

// Implementación de getLevelById
void Level::createLevelById(int id) {
    switch (id) {
        case 0:
            std::cout << "Creando nivel 0\n";
            initLevel0();
            break;
        // Agregar más casos aquí para otros niveles si es necesario
        case 1:
            std::cout << "Creando nivel 1\n";
            initLevel1();
            break;
        case 2:
            std::cout << "Creando nivel 2\n";
            initLevel2();
            break;
        default:
            initLevel0();  // Nivel predeterminado si el ID no coincide
    }
}


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


// Implementación de getLevel0
void Level::initLevel1() {
    levelState.num_ducks = 0;  // Ejemplo: 4 patos en este nivel
    for (int i = 0; i < levelState.num_ducks; ++i)
        levelState.ducks[i] = {};

    // Configuración de plataformas 
    levelState.num_platforms = 20;
    for (int i = 0; i < 20; ++i) {
        levelState.platforms[i].pos = {32*5 + i * WIDTH_PLATFORM, 200};
        levelState.platforms[i].type = GRASS_PLATFORM;
    }

    levelState.num_platforms += 10;
    for (int i = 20; i < 30; ++i) {
        levelState.platforms[i].pos = {i * WIDTH_PLATFORM - 20*WIDTH_PLATFORM, 300};
        levelState.platforms[i].type = GRASS_PLATFORM;
    }

    levelState.num_platforms += 7;
    for (int i = 30; i < 37; ++i) {
        levelState.platforms[i].pos = {i * WIDTH_PLATFORM - 18*WIDTH_PLATFORM, 300};
        levelState.platforms[i].type = GRASS_PLATFORM;
    }

    levelState.num_platforms += 10;
    for (int i = 37; i < 47; ++i) {
        levelState.platforms[i].pos = {i * WIDTH_PLATFORM - 16*WIDTH_PLATFORM, 300};
        levelState.platforms[i].type = GRASS_PLATFORM;
    }



    levelState.num_platforms += 15;
    for (int i = 47; i < 62; ++i) {
        int offset = i * WIDTH_PLATFORM - 44*WIDTH_PLATFORM + 2*WIDTH_PLATFORM*(i-47) - WIDTH_PLATFORM;
        if (offset < 20*WIDTH_PLATFORM){
            levelState.platforms[i].pos = {offset, 500};
            levelState.platforms[i].type = GRASS_PLATFORM;
        } else if ( offset == 20*WIDTH_PLATFORM) {
            continue;
        } else {
            levelState.platforms[i].pos = {offset - WIDTH_PLATFORM, 500};
            levelState.platforms[i].type = GRASS_PLATFORM;
        }
 
    }

    levelState.num_platforms += 7;
    for (int i = 62; i < 69; ++i) {
        if (i < 66) {
            levelState.platforms[i].pos = {i * WIDTH_PLATFORM - 60*WIDTH_PLATFORM , 500};
            levelState.platforms[i].type = GRASS_PLATFORM;
        } else {
            levelState.platforms[i].pos = {i * WIDTH_PLATFORM - 40*WIDTH_PLATFORM , 500};
            levelState.platforms[i].type = GRASS_PLATFORM;
        }

    }


    levelState.num_platforms += 4;
    for (int i = 69; i < 73; ++i) {
        // pongo bloques en x = 11, 12, 19,20
        if (i < 71) {
            levelState.platforms[i].pos = {i * WIDTH_PLATFORM - 59*WIDTH_PLATFORM, 400};
            levelState.platforms[i].type = GRASS_PLATFORM;
        } else {
            levelState.platforms[i].pos = {i * WIDTH_PLATFORM - 52*WIDTH_PLATFORM, 400};
            levelState.platforms[i].type = GRASS_PLATFORM;
        }

    }


    levelState.num_platforms += 8;
    for (int i = 73; i < 81; ++i) {
        levelState.platforms[i].pos = {i * WIDTH_PLATFORM - 61*WIDTH_PLATFORM, 500};
        levelState.platforms[i].type = GRASS_PLATFORM;
    }

    //          --------------------------
    //  --------------  -----  -----------------                
    //  -----------------------------  -  -  -----
    // Configuración de lugares de aparición (spawn)
    levelState.num_spawn_places = 10;
    
    levelState.spawn_places[0].pos = {14*32, 140};
    levelState.spawn_places[0].is_active = true;
    levelState.spawn_places[0].weapon = null_weapon;
    levelState.spawn_places[0].armor = null_armor;
    
    levelState.spawn_places[1].pos = {15*32, 140};
    levelState.spawn_places[1].is_active = true;
    levelState.spawn_places[1].weapon = null_weapon;
    levelState.spawn_places[1].armor = null_armor;

    levelState.spawn_places[2].pos = {16*32, 140};
    levelState.spawn_places[2].is_active = true;
    levelState.spawn_places[2].weapon = null_weapon;
    levelState.spawn_places[2].armor = null_armor;

    levelState.spawn_places[3].pos = {17*32, 140}; // no mas de cuatro jugadores.
    levelState.spawn_places[3].is_active = true;
    levelState.spawn_places[3].weapon = null_weapon;
    levelState.spawn_places[3].armor = null_armor;

    std::cout << levelState.spawn_places[0].pos.x << " " << levelState.spawn_places[0].pos.y << std::endl;
    levelState.spawn_places[4] = getRandomSpawnPlace(100, 467);
    levelState.spawn_places[5] = getRandomSpawnPlace(150, 467);
    levelState.spawn_places[6] = getRandomSpawnPlace(830, 467);
    levelState.spawn_places[7] = getRandomSpawnPlace(880, 467);
    levelState.spawn_places[8] = getRandomSpawnPlace(460, 267);
    levelState.spawn_places[9] = getRandomSpawnPlace(510, 267);

    // Inicialización de cajas y proyectiles
    this->boxes.clear();
    this->boxes.push_back(getRandomBox(100, 294));
    this->boxes.push_back(getRandomBox(150, 294));
    this->boxes.push_back(getRandomBox(830, 294));
    this->boxes.push_back(getRandomBox(880, 294));
    this->boxes.push_back(getRandomBox(480, 494)); 
    this->boxes.push_back(getRandomBox(530, 494)); 
    
    levelState.num_boxes = 6;
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


// Implementación de getLevel0
void Level::initLevel2() {
    levelState.num_ducks = 0;  // Ejemplo: 4 patos en este nivel
    for (int i = 0; i < levelState.num_ducks; ++i)
        levelState.ducks[i] = {};


    // Configuración de plataformas 
    levelState.num_platforms = 22;
    for (int i = 0; i < 20; ++i) {
        if ( 5 <= i && i < 15) {
            levelState.platforms[i].pos = {32*5 + i * WIDTH_PLATFORM, 400 + HEIGHT_PLATFORM};
            levelState.platforms[i].type = GRASS_PLATFORM;
        } else {
            levelState.platforms[i].pos = {32*5 + i * WIDTH_PLATFORM, 400};
            levelState.platforms[i].type = GRASS_PLATFORM;
        }
    }
    levelState.platforms[20].pos = { 9 * WIDTH_PLATFORM, 400 + HEIGHT_PLATFORM};
    levelState.platforms[20].type = DIRT_PLATFORM;

    levelState.platforms[21].pos = {20 * WIDTH_PLATFORM, 400 + HEIGHT_PLATFORM};
    levelState.platforms[21].type = DIRT_PLATFORM;

    // empiezo desde el 9
    levelState.num_platforms += 16;
    for (int i = 22; i < 38; ++i) {
        if ( 25 <= i && i < 35) {
            levelState.platforms[i].pos = { i * WIDTH_PLATFORM - 15*WIDTH_PLATFORM, 275};
            levelState.platforms[i].type = GRASS_PLATFORM;
        } else {
            levelState.platforms[i].pos = { i * WIDTH_PLATFORM - 15*WIDTH_PLATFORM, 275 + HEIGHT_PLATFORM};
            levelState.platforms[i].type = GRASS_PLATFORM;
        }
    }

    levelState.num_platforms += 4;
    levelState.platforms[38].pos = { 10 * WIDTH_PLATFORM, 275 + HEIGHT_PLATFORM};
    levelState.platforms[38].type = DIRT_PLATFORM;

    levelState.platforms[39].pos = { 19 * WIDTH_PLATFORM, 275 + HEIGHT_PLATFORM};
    levelState.platforms[39].type = DIRT_PLATFORM;

    levelState.platforms[40].pos = { 4 * WIDTH_PLATFORM, 400};
    levelState.platforms[40].type = GRASS_PLATFORM;

    levelState.platforms[41].pos = { 25 * WIDTH_PLATFORM, 400};
    levelState.platforms[41].type = GRASS_PLATFORM;

    levelState.num_platforms += 4;
    for (int i = 42; i < 46; ++i) {
        if (i < 44){
            levelState.platforms[i].pos = { i * WIDTH_PLATFORM - 35*WIDTH_PLATFORM, 150 + HEIGHT_PLATFORM};
            levelState.platforms[i].type = GRASS_PLATFORM;
        } else {
            levelState.platforms[i].pos = { i * WIDTH_PLATFORM - 23*WIDTH_PLATFORM, 150 + HEIGHT_PLATFORM};
            levelState.platforms[i].type = GRASS_PLATFORM;
        }
    }



    //          --------------------------
    //  --------------  -----  -----------------                
    //  -----------------------------  -  -  -----
    // Configuración de lugares de aparición (spawn)
    levelState.num_spawn_places = 10;
    
    levelState.spawn_places[0].pos = {5*32, 360};
    levelState.spawn_places[0].is_active = true;
    levelState.spawn_places[0].weapon = null_weapon;
    levelState.spawn_places[0].armor = null_armor;
    
    levelState.spawn_places[1].pos = {25*32, 360};
    levelState.spawn_places[1].is_active = true;
    levelState.spawn_places[1].weapon = null_weapon;
    levelState.spawn_places[1].armor = null_armor;

    levelState.spawn_places[2].pos = {6*32, 360};
    levelState.spawn_places[2].is_active = true;
    levelState.spawn_places[2].weapon = null_weapon;
    levelState.spawn_places[2].armor = null_armor;

    levelState.spawn_places[3].pos = {26*32, 360}; // no mas de cuatro jugadores.
    levelState.spawn_places[3].is_active = true;
    levelState.spawn_places[3].weapon = null_weapon;
    levelState.spawn_places[3].armor = null_armor;

    std::cout << levelState.spawn_places[0].pos.x << " " << levelState.spawn_places[0].pos.y << std::endl;
    levelState.spawn_places[4] = getRandomSpawnPlace(430, 367 + HEIGHT_PLATFORM);
    levelState.spawn_places[5] = getRandomSpawnPlace(510, 367 + HEIGHT_PLATFORM);
    levelState.spawn_places[6] = getRandomSpawnPlace(430, 242);
    levelState.spawn_places[7] = getRandomSpawnPlace(510, 242);
    levelState.spawn_places[8] = getRandomSpawnPlace(247, 117 + HEIGHT_PLATFORM);
    levelState.spawn_places[9] = getRandomSpawnPlace(695, 117 + HEIGHT_PLATFORM);

    // Inicialización de cajas y proyectiles
    this->boxes.clear();
    this->boxes.push_back(getRandomBox(200, 394));
    this->boxes.push_back(getRandomBox(250, 394));
    this->boxes.push_back(getRandomBox(360, 267));
    this->boxes.push_back(getRandomBox(624, 267));
    this->boxes.push_back(getRandomBox(734, 394)); 
    this->boxes.push_back(getRandomBox(784, 394)); 
    
    levelState.num_boxes = 6;
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

