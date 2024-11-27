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
            initLevel0();
            break;
        case 1:
            initLevel1();
            break;
        case 2:
            initLevel2();
            break;
        case 3:
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
    levelState.num_bananas = 0;

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

    for (auto& banana : levelState.bananas)
        banana = {0, 0};
}

void Level::initLevel0() {
    clearLevelState(); 

    levelState.num_platforms = 29;
    for (int i = 0; i < 29; ++i) {
        levelState.platforms[i] = {{i * WIDTH_PLATFORM+64, 650}, GRASS_PLATFORM};
    }

    levelState.num_platforms += 14;
    for (int i = 29; i < 43; ++i) {
        levelState.platforms[i] = {{i * WIDTH_PLATFORM - 21*WIDTH_PLATFORM+72, 550}, GRASS_PLATFORM};
    }

    levelState.num_platforms += 14;
    for (int i = 43; i < 57; ++i) {
        if (i < 50) 
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 42*WIDTH_PLATFORM+80, 450}, GRASS_PLATFORM};
        else 
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 27*WIDTH_PLATFORM+64, 450}, GRASS_PLATFORM};
    }


    levelState.num_platforms += 14;
    for (int i = 57; i < 71; ++i) {
        if (i < 64) {
            levelState.platforms[i].pos = {i * WIDTH_PLATFORM - 47*WIDTH_PLATFORM+32, 300-i*HEIGHT_PLATFORM+58*HEIGHT_PLATFORM +20};
        } else {
            levelState.platforms[i].pos = {i * WIDTH_PLATFORM - 47*WIDTH_PLATFORM+32, 300+i*HEIGHT_PLATFORM-69*HEIGHT_PLATFORM + 20};
        }
        levelState.platforms[i].type = GRASS_PLATFORM;
    }
    // a cada plataforma le bajo 1.
    for (int i = 57; i < 64; ++i) {
        levelState.platforms[i].pos.y += i-57;
    }
    
    levelState.platforms[70].pos.y += 0;
    levelState.platforms[69].pos.y += 2;
    levelState.platforms[68].pos.y += 3;
    levelState.platforms[67].pos.y += 4;
    levelState.platforms[66].pos.y += 5;
    levelState.platforms[65].pos.y += 6;
    levelState.platforms[64].pos.y += 6;


    levelState.num_platforms += 12;
    for(int i = 71; i < 83; ++i) {
        levelState.platforms[i].pos = {i*WIDTH_PLATFORM -60*WIDTH_PLATFORM + 29+16, 332 + 20};
        levelState.platforms[i].type = DIRT_PLATFORM;
    }

    levelState.num_platforms += 10;
    for(int i = 83; i < 93; ++i) {
        levelState.platforms[i].pos = {i*WIDTH_PLATFORM -70*WIDTH_PLATFORM + 8 + 16, 301 + 20};
        levelState.platforms[i].type = DIRT_PLATFORM;
    }

    levelState.num_platforms += 8;
    for(int i = 93; i < 101; ++i) { 
        levelState.platforms[i].pos = {i*WIDTH_PLATFORM -79*WIDTH_PLATFORM + 17+ 16, 270 + 20};
        levelState.platforms[i].type = DIRT_PLATFORM;
    }

    levelState.num_platforms += 6;
    for(int i = 101; i < 107; ++i) {
        levelState.platforms[i].pos = {i*WIDTH_PLATFORM -86*WIDTH_PLATFORM + 24+ 16, 239 + 20};
        levelState.platforms[i].type = DIRT_PLATFORM;
    }

    levelState.num_platforms += 4;
    for(int i = 107; i < 111; ++i) {
        levelState.platforms[i].pos = {i*WIDTH_PLATFORM -91*WIDTH_PLATFORM + 29+ 16, 208 + 20};
        levelState.platforms[i].type = DIRT_PLATFORM;
    }

    levelState.num_platforms += 2;
    for(int i = 111; i < 113; ++i) {
        levelState.platforms[i].pos = {i*WIDTH_PLATFORM -93*WIDTH_PLATFORM+ 16, 180 + 17};
        levelState.platforms[i].type = DIRT_PLATFORM;
    }

    for (int i = 0; i < levelState.num_platforms; ++i) 
        levelState.platforms[i].pos = {levelState.platforms[i].pos.x - i, levelState.platforms[i].pos.y};
    

    levelState.num_spawn_places = 8;
    
    levelState.spawn_places[0] = { {150, 500}, true, nullWeapon, nullArmor };
    levelState.spawn_places[1] = { {200, 500}, true, nullWeapon, nullArmor };
    levelState.spawn_places[2] = { {820, 500}, true, nullWeapon, nullArmor };
    levelState.spawn_places[3] = { {870, 500}, true, nullWeapon, nullArmor };
    levelState.spawn_places[4] = getRandomSpawnPlace(167, 418);
    levelState.spawn_places[5] = getRandomSpawnPlace(470, 518);
    levelState.spawn_places[6] = getRandomSpawnPlace(550, 518);
    levelState.spawn_places[7] = getRandomSpawnPlace(848, 418);

    this->spawns.clear();
    this->spawns.push_back(std::make_unique<Spawn>(150, 500, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(200, 500, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(820, 500, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(870, 500, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(167, 418, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(470, 518, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(550, 518, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(848, 418, true, false, 0.0f));



    // Inicialización de cajas y proyectiles
    this->boxes.clear();
    this->boxes.push_back(getRandomBox(525, 159));
    this->boxes.push_back(getRandomBox(375, 642));
    this->boxes.push_back(getRandomBox(475, 642));
    this->boxes.push_back(getRandomBox(575, 642));
    this->boxes.push_back(getRandomBox(675, 642)); 
    


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

    //         --------------------------
    // ---------------   ------   ------------------


// 22
    
    levelState.num_platforms += 4;
    for (int i = 47; i < 51; ++i) {
        // pongo bloques en x = 11, 12, 19,20
        if (i < 49) {
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 37*WIDTH_PLATFORM + 15, 400}, GRASS_PLATFORM};
        } else {
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 30*WIDTH_PLATFORM + 12, 400}, GRASS_PLATFORM};
        }
    }

    levelState.num_platforms += 6;
    for (int i = 51; i < 57; ++i) {
        // pongo bloques en x = 4, 5, 6, 7
        levelState.platforms[i] = {{i * WIDTH_PLATFORM - 48*WIDTH_PLATFORM + 4, 500}, GRASS_PLATFORM};
    }

    levelState.num_platforms += 7;
    for (int i = 57; i < 64; ++i) {
        levelState.platforms[i] = {{i * WIDTH_PLATFORM - 44*WIDTH_PLATFORM - 9, 500}, GRASS_PLATFORM};
    }

    levelState.num_platforms += 6;
    for (int i = 64; i < 70; ++i) {
        levelState.platforms[i] = {{i * WIDTH_PLATFORM - 40*WIDTH_PLATFORM - 16, 500}, GRASS_PLATFORM};
    }

    for (int i = 0; i < levelState.num_platforms; ++i) 
        levelState.platforms[i].pos = {levelState.platforms[i].pos.x - i, levelState.platforms[i].pos.y};

    
    //          --------------------------

    //  --------------  -----  -----------------                
    //  -----------------------------  -  -  -----
    // Configuración de lugares de aparición (spawn)
    levelState.num_spawn_places = 10;
    
    levelState.spawn_places[0] = {{13*32, 140}, true, nullWeapon, nullArmor};
    levelState.spawn_places[1] = {{14*32, 140}, true, nullWeapon, nullArmor};
    levelState.spawn_places[2] = {{15*32, 140}, true, nullWeapon, nullArmor};
    levelState.spawn_places[3] = {{16*32, 140}, true, nullWeapon, nullArmor};

    levelState.spawn_places[4] = getRandomSpawnPlace(114, 468);
    levelState.spawn_places[5] = getRandomSpawnPlace(164, 468);
    levelState.spawn_places[6] = getRandomSpawnPlace(750, 468);
    levelState.spawn_places[7] = getRandomSpawnPlace(800, 468);
    levelState.spawn_places[8] = getRandomSpawnPlace(432, 268);
    levelState.spawn_places[9] = getRandomSpawnPlace(482, 268);

    this->spawns.clear();
    this->spawns.push_back(std::make_unique<Spawn>(13*32, 140, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(14*32, 140, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(15*32, 140, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(16*32, 140, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(114, 468, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(164, 468, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(750, 468, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(800, 468, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(432, 268, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(482, 268, true, false, 0.0f));

    // Inicialización de cajas y proyectiles
    this->boxes.clear();
    this->boxes.push_back(getRandomBox(140, 294));
    this->boxes.push_back(getRandomBox(190, 294));
    this->boxes.push_back(getRandomBox(774, 294));
    this->boxes.push_back(getRandomBox(824, 294));
    this->boxes.push_back(getRandomBox(14*32, 494)); 
    this->boxes.push_back(getRandomBox(15*33, 494)); 
    
    levelState.num_boxes = 6;
    for (int i = 0; i < levelState.num_boxes; ++i)
        levelState.boxes[i] = boxes[i]->getBoxState();

}

void Level::initLevel2() {
    clearLevelState();
 
    levelState.num_platforms = 20;
    for (int i = 0; i < 20; ++i) {
        if ( 5 <= i && i < 15) {
            levelState.platforms[i] = {{32*5 + i * WIDTH_PLATFORM, 400 + HEIGHT_PLATFORM}, GRASS_PLATFORM};
        } else {
            levelState.platforms[i] = {{32*5 + i * WIDTH_PLATFORM, 400+1}, GRASS_PLATFORM};
        }
    }


    // empiezo desde el 9
    levelState.num_platforms += 16;
    for (int i = 20; i < 36; ++i) {
        if ( 23 <= i && i < 33) 
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 12*WIDTH_PLATFORM - 16, 275 +1}, GRASS_PLATFORM};
        else 
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 12*WIDTH_PLATFORM - 16, 275 + HEIGHT_PLATFORM}, GRASS_PLATFORM};
    }

    levelState.num_platforms += 4;
    for (int i = 36; i < 40; ++i) {
        if (i < 38)
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 29*WIDTH_PLATFORM, 150 + HEIGHT_PLATFORM}, GRASS_PLATFORM};
        else 
            levelState.platforms[i] = {{i * WIDTH_PLATFORM - 15*WIDTH_PLATFORM - 14, 150 + HEIGHT_PLATFORM}, GRASS_PLATFORM};      
    }


    for (int i = 0; i < levelState.num_platforms; ++i) 
        levelState.platforms[i].pos = {levelState.platforms[i].pos.x - i, levelState.platforms[i].pos.y};

    
    levelState.platforms[levelState.num_platforms++] = {{22* WIDTH_PLATFORM - 12*WIDTH_PLATFORM - 7, 275+HEIGHT_PLATFORM }, DIRT_PLATFORM};
    levelState.platforms[levelState.num_platforms++] = {{19* WIDTH_PLATFORM - 16, 275+HEIGHT_PLATFORM }, DIRT_PLATFORM};
    levelState.platforms[levelState.num_platforms++] = {{9* WIDTH_PLATFORM - 4, 400+HEIGHT_PLATFORM }, DIRT_PLATFORM};
    levelState.platforms[levelState.num_platforms++] = {{20* WIDTH_PLATFORM - 15, 400+HEIGHT_PLATFORM }, DIRT_PLATFORM};


    // Configuración de lugares de aparición (spawn)
    levelState.num_spawn_places = 10;
    
    levelState.spawn_places[0] = {{6*32, 360}, true, nullWeapon, nullArmor};
    levelState.spawn_places[1] = {{24*32-16, 360}, true, nullWeapon, nullArmor};
    levelState.spawn_places[2] = {{7*32, 360}, true, nullWeapon, nullArmor};
    levelState.spawn_places[3] = {{23*32-16, 360}, true, nullWeapon, nullArmor};


    levelState.spawn_places[4] = getRandomSpawnPlace(414, 367 + HEIGHT_PLATFORM);
    levelState.spawn_places[5] = getRandomSpawnPlace(510, 367 + HEIGHT_PLATFORM);
    levelState.spawn_places[6] = getRandomSpawnPlace(414, 243);
    levelState.spawn_places[7] = getRandomSpawnPlace(510, 243);
    levelState.spawn_places[8] = getRandomSpawnPlace(211, 117 + HEIGHT_PLATFORM);
    levelState.spawn_places[9] = getRandomSpawnPlace(707, 117 + HEIGHT_PLATFORM);

    this->spawns.clear();
    this->spawns.push_back(std::make_unique<Spawn>(5*32, 360, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(25*32, 360, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(6*32, 360, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(26*32, 360, false, true, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(430, 367 + HEIGHT_PLATFORM, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(510, 367 + HEIGHT_PLATFORM, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(430, 243, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(510, 243, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(247, 117 + HEIGHT_PLATFORM, true, false, 0.0f));
    this->spawns.push_back(std::make_unique<Spawn>(695, 117 + HEIGHT_PLATFORM, true, false, 0.0f));


    // Inicialización de cajas y proyectiles
    this->boxes.clear();
    this->boxes.push_back(getRandomBox(255, 394));
    this->boxes.push_back(getRandomBox(305, 394));
    this->boxes.push_back(getRandomBox(343, 268));
    this->boxes.push_back(getRandomBox(616, 268));
    this->boxes.push_back(getRandomBox(658, 394));
    this->boxes.push_back(getRandomBox(708, 394)); 
    
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
        levelState.platforms[i] = {{32*7 + i * WIDTH_PLATFORM - i, 400}, GRASS_PLATFORM};
        if(i == 0 || i == 14)
            levelState.platforms[i].type = DIRT_PLATFORM;
    }
    levelState.num_platforms += 5;
    for(int i = 15; i < 20; ++i){
        levelState.platforms[i] = {{32*7, 400 - 32*i + 32*14 + i - 14}, DIRT_PLATFORM};
        if(i == 19)
            levelState.platforms[i].type = GRASS_PLATFORM;
    }

    levelState.num_platforms += 5;
    for(int i = 20; i < 25; ++i){
        levelState.platforms[i] = {{32*21 - 14, 400 - 32*i + 32*19 + i - 19}, DIRT_PLATFORM};
        if(i == 24)
            levelState.platforms[i].type = GRASS_PLATFORM;
    }

    levelState.num_platforms += 13;
    for (int i = 25; i < 38; ++i) 
        levelState.platforms[i] = {{32*7 + i * WIDTH_PLATFORM - 24*WIDTH_PLATFORM -i + 24, 400 - 32*5+5}, GRASS_PLATFORM};
    

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
