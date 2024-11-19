#include "level.h"


Level::Level()  {
    std::random_device rd;  // Obtiene una semilla del hardware
    rng = std::mt19937(rd());
    boxDist = std::uniform_int_distribution<int>(1, MAX_BOXES);
    weaponDist = std::uniform_int_distribution<int>(20, 29);
    armorDist = std::uniform_int_distribution<int>(30, 31);
    // hago un random id:
    std::uniform_int_distribution<int> levelDist(0, 2);
    chosenLevel = levelDist(rng);
    createLevelById(chosenLevel);
}

void Level::createNewLevel(){
    int id = chosenLevel;
    std::uniform_int_distribution<int> levelDist(0, 2);
    while (id == chosenLevel) 
        id = levelDist(rng);
    
    chosenLevel = id;
    createLevelById(id);
}

void Level::initWinningLevel() {
    chosenLevel = 3;
    winningLevel();
}

void Level::createLevelById(int id) {
    switch (id) {
        case 0:
            std::cout << "Creando nivel 0\n";
            initLevel0();
            break;
        case 1:
            std::cout << "Creando nivel 1\n";
            initLevel1();
            break;
        case 2:
            std::cout << "Creando nivel 2\n";
            initLevel2();
            break;
        case 3:
            std::cout << "Creando nivel ganador\n";
            winningLevel();
            break;
        default:
            initLevel0();  
    }
}


void Level::clearLevelState() {
    levelState.num_ducks = 0;
    levelState.num_platforms = 0;
    levelState.num_spawn_places = 0;
    levelState.num_boxes = 0;
    levelState.num_projectiles = 0;
    levelState.num_dropped_weapons = 0;
    levelState.num_dropped_armors = 0;

    for (auto& duck : levelState.ducks)
        duck = {{0, 0}, 0, false, false, false, false, false, 0, false, 0, 0, nullWeapon, nullArmor, nullArmor};

    for (auto& platform : levelState.platforms)
        platform = {{0, 0}, 0};

    for (auto& spawn : levelState.spawn_places) {
        spawn.pos = {0, 0};
        spawn.is_active = false;
        spawn.weapon = nullWeapon;
        spawn.armor = nullArmor;
    }

    for (auto& box : levelState.boxes)
        box = {{0, 0}, 0, false};

    for (auto& projectile : levelState.projectiles)
        projectile = {{0, 0}, 0, false};

    for (auto& weapon : levelState.dropped_weapons)
        weapon = nullWeapon;

    for (auto& armor : levelState.dropped_armors)
        armor = nullArmor;
}

void Level::initLevel0() {
    clearLevelState(); 

    levelState.num_platforms = 30;
    for (int i = 0; i < 30; ++i) {
        levelState.platforms[i] = {{i * WIDTH_PLATFORM, 650}, GRASS_PLATFORM};
    }

    levelState.num_platforms += 14;
    for (int i = 30; i < 44; ++i) {
        levelState.platforms[i] = {{i * WIDTH_PLATFORM - 22*WIDTH_PLATFORM, 550}, GRASS_PLATFORM};
    }

    levelState.num_platforms += 14;
    for (int i = 44; i < 58; ++i) {
        if (i < 51) 
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 44*WIDTH_PLATFORM, 450}, GRASS_PLATFORM};
        else 
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 28*WIDTH_PLATFORM, 450}, GRASS_PLATFORM};
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



    levelState.num_spawn_places = 8;
    
    levelState.spawn_places[0] = { {50, 500}, true, nullWeapon, nullArmor };
    levelState.spawn_places[1] = { {100, 500}, true, nullWeapon, nullArmor };
    levelState.spawn_places[2] = { {874, 500}, true, nullWeapon, nullArmor };
    levelState.spawn_places[3] = { {924, 500}, true, nullWeapon, nullArmor };
    levelState.spawn_places[4] = getRandomSpawnPlace(75, 418);
    levelState.spawn_places[5] = getRandomSpawnPlace(425, 518);
    levelState.spawn_places[6] = getRandomSpawnPlace(510, 518);
    levelState.spawn_places[7] = getRandomSpawnPlace(875, 418);

    this->spawns.clear();
    this->spawns.push_back(std::make_unique<Spawn>(50, 500, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(100, 500, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(874, 500, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(924, 500, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(75, 418, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(425, 518, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(510, 518, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(875, 418, true, false, 0.0f));



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


}

void Level::initLevel1() {
    clearLevelState();

    levelState.num_platforms = 20;
    for (int i = 0; i < 20; ++i) {
        levelState.platforms[i] = {{32*5 + i * WIDTH_PLATFORM, 200}, GRASS_PLATFORM};
    }

    levelState.num_platforms += 10;
    for (int i = 20; i < 30; ++i) {
        levelState.platforms[i] = {{i * WIDTH_PLATFORM - 20*WIDTH_PLATFORM, 300}, GRASS_PLATFORM};
    }

    levelState.num_platforms += 7;
    for (int i = 30; i < 37; ++i) {
        levelState.platforms[i] = {{i * WIDTH_PLATFORM - 18*WIDTH_PLATFORM, 300}, GRASS_PLATFORM};
    }

    levelState.num_platforms += 10;
    for (int i = 37; i < 47; ++i) {
        levelState.platforms[i] = {{i * WIDTH_PLATFORM - 16*WIDTH_PLATFORM, 300}, GRASS_PLATFORM};
    }



    levelState.num_platforms += 15;
    for (int i = 47; i < 62; ++i) {
        int offset = i * WIDTH_PLATFORM - 44*WIDTH_PLATFORM + 2*WIDTH_PLATFORM*(i-47) - WIDTH_PLATFORM;
        if (offset < 20*WIDTH_PLATFORM){
            levelState.platforms[i] = {{offset, 500}, GRASS_PLATFORM};
        } else if ( offset == 20*WIDTH_PLATFORM) {
            continue;
        } else {
            levelState.platforms[i] = {{offset - WIDTH_PLATFORM, 500}, GRASS_PLATFORM};
        }
 
    }

    levelState.num_platforms += 7;
    for (int i = 62; i < 69; ++i) {
        if (i < 66) {
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 60*WIDTH_PLATFORM, 500}, GRASS_PLATFORM};
        } else {
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 40*WIDTH_PLATFORM, 500}, GRASS_PLATFORM};
        }

    }


    levelState.num_platforms += 4;
    for (int i = 69; i < 73; ++i) {
        // pongo bloques en x = 11, 12, 19,20
        if (i < 71) {
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 59*WIDTH_PLATFORM, 400}, GRASS_PLATFORM};
        } else {
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 52*WIDTH_PLATFORM, 400}, GRASS_PLATFORM};
        }
    }


    levelState.num_platforms += 8;
    for (int i = 73; i < 81; ++i) {
        levelState.platforms[i] = {{i * WIDTH_PLATFORM - 61*WIDTH_PLATFORM, 500}, GRASS_PLATFORM};
    }

    //          --------------------------
    //  --------------  -----  -----------------                
    //  -----------------------------  -  -  -----
    // Configuración de lugares de aparición (spawn)
    levelState.num_spawn_places = 10;
    
    levelState.spawn_places[0] = {{14*32, 140}, true, nullWeapon, nullArmor};
    levelState.spawn_places[1] = {{15*32, 140}, true, nullWeapon, nullArmor};
    levelState.spawn_places[2] = {{16*32, 140}, true, nullWeapon, nullArmor};
    levelState.spawn_places[3] = {{17*32, 140}, true, nullWeapon, nullArmor};

    levelState.spawn_places[4] = getRandomSpawnPlace(100, 467);
    levelState.spawn_places[5] = getRandomSpawnPlace(150, 467);
    levelState.spawn_places[6] = getRandomSpawnPlace(830, 467);
    levelState.spawn_places[7] = getRandomSpawnPlace(880, 467);
    levelState.spawn_places[8] = getRandomSpawnPlace(460, 267);
    levelState.spawn_places[9] = getRandomSpawnPlace(510, 267);

    this->spawns.clear();
    this->spawns.push_back(std::make_unique<Spawn>(14*32, 140, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(15*32, 140, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(16*32, 140, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(17*32, 140, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(100, 467, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(150, 467, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(830, 467, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(880, 467, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(460, 267, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(510, 267, true, false, 0.0f));

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

}

void Level::initLevel2() {
    clearLevelState();
 
    levelState.num_platforms = 22;
    for (int i = 0; i < 20; ++i) {
        if ( 5 <= i && i < 15) {
            levelState.platforms[i] = {{32*5 + i * WIDTH_PLATFORM, 400 + HEIGHT_PLATFORM}, GRASS_PLATFORM};
        } else {
            levelState.platforms[i] = {{32*5 + i * WIDTH_PLATFORM, 400}, GRASS_PLATFORM};
        }
    }

    levelState.platforms[20] = {{9 * WIDTH_PLATFORM, 400 + HEIGHT_PLATFORM}, DIRT_PLATFORM};
    levelState.platforms[21] = {{20 * WIDTH_PLATFORM, 400 + HEIGHT_PLATFORM}, DIRT_PLATFORM};

    // empiezo desde el 9
    levelState.num_platforms += 16;
    for (int i = 22; i < 38; ++i) {
        if ( 25 <= i && i < 35) 
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 15*WIDTH_PLATFORM, 275 }, GRASS_PLATFORM};
        else 
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 15*WIDTH_PLATFORM, 275 + HEIGHT_PLATFORM}, GRASS_PLATFORM};
    }

    levelState.num_platforms += 4;
    levelState.platforms[38] = {{10 * WIDTH_PLATFORM, 275 + HEIGHT_PLATFORM}, DIRT_PLATFORM};
    levelState.platforms[39] = {{19 * WIDTH_PLATFORM, 275 + HEIGHT_PLATFORM}, DIRT_PLATFORM};
    levelState.platforms[40] = {{4 * WIDTH_PLATFORM, 400}, GRASS_PLATFORM};
    levelState.platforms[41] = {{25 * WIDTH_PLATFORM, 400}, GRASS_PLATFORM};

    levelState.num_platforms += 4;
    for (int i = 42; i < 46; ++i) {
        if (i < 44)
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 35*WIDTH_PLATFORM, 150 + HEIGHT_PLATFORM}, GRASS_PLATFORM};
        else 
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 23*WIDTH_PLATFORM, 150 + HEIGHT_PLATFORM}, GRASS_PLATFORM};      
    }



    //          --------------------------
    //  --------------  -----  -----------------                
    //  -----------------------------  -  -  -----
    // Configuración de lugares de aparición (spawn)
    levelState.num_spawn_places = 10;
    
    levelState.spawn_places[0] = {{5*32, 360}, true, nullWeapon, nullArmor};
    levelState.spawn_places[1] = {{25*32, 360}, true, nullWeapon, nullArmor};
    levelState.spawn_places[2] = {{6*32, 360}, true, nullWeapon, nullArmor};
    levelState.spawn_places[3] = {{26*32, 360}, true, nullWeapon, nullArmor};


    levelState.spawn_places[4] = getRandomSpawnPlace(430, 367 + HEIGHT_PLATFORM);
    levelState.spawn_places[5] = getRandomSpawnPlace(510, 367 + HEIGHT_PLATFORM);
    levelState.spawn_places[6] = getRandomSpawnPlace(430, 242);
    levelState.spawn_places[7] = getRandomSpawnPlace(510, 242);
    levelState.spawn_places[8] = getRandomSpawnPlace(247, 117 + HEIGHT_PLATFORM);
    levelState.spawn_places[9] = getRandomSpawnPlace(695, 117 + HEIGHT_PLATFORM);

    this->spawns.clear();
    this->spawns.push_back(std::make_unique<Spawn>(5*32, 360, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(25*32, 360, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(6*32, 360, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(26*32, 360, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(430, 367 + HEIGHT_PLATFORM, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(510, 367 + HEIGHT_PLATFORM, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(430, 242, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(510, 242, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(247, 117 + HEIGHT_PLATFORM, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(695, 117 + HEIGHT_PLATFORM, true, false, 0.0f));


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

}

// Implementación de getLevel0
void Level::winningLevel() {
    clearLevelState();

    // Configuración de plataformas 
    levelState.num_platforms = 15;
    for (int i = 0; i < 15; ++i) {
        levelState.platforms[i] = {{32*7 + i * WIDTH_PLATFORM, 400}, GRASS_PLATFORM};
        if(i == 0 || i == 14)
            levelState.platforms[i].type = DIRT_PLATFORM;
    }
    levelState.num_platforms += 5;
    for(int i = 15; i < 20; ++i){
        levelState.platforms[i] = {{32*7, 400 - 32*i + 32*14}, DIRT_PLATFORM};
        if(i == 19)
            levelState.platforms[i].type = GRASS_PLATFORM;
    }

    levelState.num_platforms += 5;
    for(int i = 20; i < 25; ++i){
        levelState.platforms[i] = {{32*21, 400 - 32*i + 32*19}, DIRT_PLATFORM};
        if(i == 24)
            levelState.platforms[i].type = GRASS_PLATFORM;
    }

    levelState.num_platforms += 13;
    for (int i = 25; i < 38; ++i) 
        levelState.platforms[i] = {{32*7 + i * WIDTH_PLATFORM - 24*WIDTH_PLATFORM, 400 - 32*5}, GRASS_PLATFORM};
    

    levelState.num_spawn_places = 4;
    this->spawns.clear();
    for (int i = 0; i < 4; ++i) {
        levelState.spawn_places[i] = {{470, 360}, true, nullWeapon, nullArmor};
        this->spawns.push_back(std::make_unique<Spawn>(470, 360, false, true, 0.0f));
    }

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
    int randomIndex = boxDist(rng);
    
    box_t box = { {x, y}, BOX_HEALTH, false };
    armor_t armor = nullArmor;
    weapon_t weapon = nullWeapon;

    if (randomIndex < MAX_BOXES / 3) {
        box.is_explosive = true;
    } else if (randomIndex < 2 * MAX_BOXES / 3) {
        armor = {{x, y}, getRandomArmor()};
    } else {
        uint8_t type = getRandomWeapon();
        weapon = {{x, y}, type, ammoForWeapons[type]};
    }

    return std::make_unique<Box>(box, armor, weapon);
}

uint8_t Level::getRandomWeapon() {
    return weaponDist(rng);
}


uint8_t Level::getRandomArmor() {
    return armorDist(rng);
}

spawn_place_t Level::getRandomSpawnPlace(int x, int y) {
    std::uniform_int_distribution<int> spawnDist(1, levelState.num_spawn_places);
    int randomIndex = spawnDist(rng);

    spawn_place_t spawn = { {x, y},false, nullWeapon, nullArmor};

    if (randomIndex < levelState.num_spawn_places / 2) {
        spawn.armor.pos = {x, y};
        spawn.armor.type = getRandomArmor();
    } else {
        spawn.weapon.pos = {x, y};
        spawn.weapon.type = getRandomWeapon();
        spawn.weapon.ammo = ammoForWeapons[spawn.weapon.type]; 
    }

    return spawn;
}

void Level::updateState(level_t& state) {
    levelState = state;
}
