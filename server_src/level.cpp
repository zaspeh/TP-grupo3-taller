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

void Level::clearLevelState() {
    levelState.num_ducks = 0;
    levelState.num_platforms = 0;
    levelState.num_spawn_places = 0;
    levelState.num_boxes = 0;
    levelState.num_projectiles = 0;
    levelState.num_dropped_weapons = 0;
    levelState.num_dropped_armors = 0;

    // Inicializamos las estructuras internas
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
    levelState.spawn_places[0].weapon = nullWeapon;
    levelState.spawn_places[0].armor = nullArmor;
    
    levelState.spawn_places[1].pos = {100, 500};
    levelState.spawn_places[1].is_active = true;
    levelState.spawn_places[1].weapon = nullWeapon;
    levelState.spawn_places[1].armor = nullArmor;

    levelState.spawn_places[2].pos = {874, 500};
    levelState.spawn_places[2].is_active = true;
    levelState.spawn_places[2].weapon = nullWeapon;
    levelState.spawn_places[2].armor = nullArmor;

    levelState.spawn_places[3].pos = {924, 500}; // no mas de cuatro jugadores.
    levelState.spawn_places[3].is_active = true;
    levelState.spawn_places[3].weapon = nullWeapon;
    levelState.spawn_places[3].armor = nullArmor;



    std::cout << levelState.spawn_places[0].pos.x << " " << levelState.spawn_places[0].pos.y << std::endl;
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


// Implementación de getLevel0
void Level::initLevel1() {
    clearLevelState();

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
    levelState.spawn_places[0].weapon = nullWeapon;
    levelState.spawn_places[0].armor = nullArmor;
    
    levelState.spawn_places[1].pos = {15*32, 140};
    levelState.spawn_places[1].is_active = true;
    levelState.spawn_places[1].weapon = nullWeapon;
    levelState.spawn_places[1].armor = nullArmor;

    levelState.spawn_places[2].pos = {16*32, 140};
    levelState.spawn_places[2].is_active = true;
    levelState.spawn_places[2].weapon = nullWeapon;
    levelState.spawn_places[2].armor = nullArmor;

    levelState.spawn_places[3].pos = {17*32, 140}; // no mas de cuatro jugadores.
    levelState.spawn_places[3].is_active = true;
    levelState.spawn_places[3].weapon = nullWeapon;
    levelState.spawn_places[3].armor = nullArmor;

    std::cout << levelState.spawn_places[0].pos.x << " " << levelState.spawn_places[0].pos.y << std::endl;
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


// Implementación de getLevel0
void Level::initLevel2() {
    clearLevelState();

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
    levelState.spawn_places[0].weapon = nullWeapon;
    levelState.spawn_places[0].armor = nullArmor;
    
    levelState.spawn_places[1].pos = {25*32, 360};
    levelState.spawn_places[1].is_active = true;
    levelState.spawn_places[1].weapon = nullWeapon;
    levelState.spawn_places[1].armor = nullArmor;

    levelState.spawn_places[2].pos = {6*32, 360};
    levelState.spawn_places[2].is_active = true;
    levelState.spawn_places[2].weapon = nullWeapon;
    levelState.spawn_places[2].armor = nullArmor;

    levelState.spawn_places[3].pos = {26*32, 360}; // no mas de cuatro jugadores.
    levelState.spawn_places[3].is_active = true;
    levelState.spawn_places[3].weapon = nullWeapon;
    levelState.spawn_places[3].armor = nullArmor;

    std::cout << levelState.spawn_places[0].pos.x << " " << levelState.spawn_places[0].pos.y << std::endl;
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

    spawn_place_t spawn = {
        {x, y},
        false,
        nullWeapon,
        nullArmor
    };

    if (randomIndex < levelState.num_spawn_places / 2) {
        spawn.armor.pos = {x, y};
        spawn.armor.type = getRandomArmor();
    } else {
        spawn.weapon.pos = {x, y};
        spawn.weapon.type = getRandomWeapon();
        spawn.weapon.ammo = ammoForWeapons[spawn.weapon.type]; // Para variar la munición inicial
    }

    return spawn;
}

void Level::updateState(level_t& state) {
    levelState = state;
}

/*
==20851== Thread 3:
==20851== Invalid read of size 8
==20851==    at 0x49364E4: std::_Rb_tree_increment(std::_Rb_tree_node_base*) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==20851==    by 0x126024: std::_Rb_tree_iterator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >::operator++() (stl_tree.h:287)
==20851==    by 0x122260: GameState::updatePlayers(float) (game_state.cpp:240)
==20851==    by 0x11BA7C: GameLoop::run() (gameloop.cpp:46)
==20851==    by 0x11466B: Thread::main() (thread.h:43)
==20851==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==20851==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==20851==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==20851==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==20851==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==20851==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==20851==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==20851==  Address 0x4f70e08 is 24 bytes inside a block of size 56 free'd
==20851==    at 0x484BB6F: operator delete(void*, unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==20851==    by 0x120311: __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::deallocate(std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >*, unsigned long) (new_allocator.h:145)
==20851==    by 0x11FFA6: std::allocator_traits<std::allocator<std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > > >::deallocate(std::allocator<std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >&, std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >*, unsigned long) (alloc_traits.h:496)
==20851==    by 0x11F9D8: std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::_M_put_node(std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >*) (stl_tree.h:565)
==20851==    by 0x11F3ED: std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::_M_drop_node(std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >*) (stl_tree.h:632)
==20851==    by 0x11E71A: std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::_M_erase(std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >*) (stl_tree.h:1891)
==20851==    by 0x128A5F: std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::clear() (stl_tree.h:1254)
==20851==    by 0x129660: std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::_M_erase_aux(std::_Rb_tree_const_iterator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::_Rb_tree_const_iterator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >) (stl_tree.h:2498)
==20851==    by 0x127122: std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::erase(unsigned char const&) (stl_tree.h:2512)
==20851==    by 0x125678: std::map<unsigned char, std::shared_ptr<PlayerState>, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::erase(unsigned char const&) (stl_map.h:1069)
==20851==    by 0x120B08: GameState::removePlayer(unsigned char) (game_state.cpp:25)
==20851==    by 0x11BCC5: GameLoop::removePlayer(unsigned char) (gameloop.cpp:75)
==20851==  Block was alloc'd at
==20851==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==20851==    by 0x12D547: __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::allocate(unsigned long, void const*) (new_allocator.h:127)
==20851==    by 0x12C5C9: std::allocator_traits<std::allocator<std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > > >::allocate(std::allocator<std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >&, unsigned long) (alloc_traits.h:464)
==20851==    by 0x12B07C: std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::_M_get_node() (stl_tree.h:561)
==20851==    by 0x128E8A: std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >* std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::_M_create_node<std::piecewise_construct_t const&, std::tuple<unsigned char const&>, std::tuple<> >(std::piecewise_construct_t const&, std::tuple<unsigned char const&>&&, std::tuple<>&&) (stl_tree.h:611)
==20851==    by 0x126F90: std::_Rb_tree_iterator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::_M_emplace_hint_unique<std::piecewise_construct_t const&, std::tuple<unsigned char const&>, std::tuple<> >(std::_Rb_tree_const_iterator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::piecewise_construct_t const&, std::tuple<unsigned char const&>&&, std::tuple<>&&) (stl_tree.h:2431)
==20851==    by 0x1255DF: std::map<unsigned char, std::shared_ptr<PlayerState>, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::operator[](unsigned char const&) (stl_map.h:501)
==20851==    by 0x120FAF: GameState::doAction(unsigned char, unsigned char) (game_state.cpp:56)
==20851==    by 0x11BC31: GameLoop::doActionGameState(unsigned char, unsigned char) (gameloop.cpp:70)
==20851==    by 0x13A13F: Receiver::run()::{lambda()#1}::operator()() const (receiver.cpp:25)
==20851==    by 0x13A8E3: void std::__invoke_impl<void, Receiver::run()::{lambda()#1}&>(std::__invoke_other, Receiver::run()::{lambda()#1}&) (invoke.h:61)
==20851==    by 0x13A7D7: std::enable_if<is_invocable_r_v<void, Receiver::run()::{lambda()#1}&>, void>::type std::__invoke_r<void, Receiver::run()::{lambda()#1}&>(Receiver::run()::{lambda()#1}&) (invoke.h:111)
==20851== 
==20851== Invalid read of size 8
==20851==    at 0x4936500: std::_Rb_tree_increment(std::_Rb_tree_node_base*) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==20851==    by 0x126024: std::_Rb_tree_iterator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >::operator++() (stl_tree.h:287)
==20851==    by 0x122260: GameState::updatePlayers(float) (game_state.cpp:240)
==20851==    by 0x11BA7C: GameLoop::run() (gameloop.cpp:46)
==20851==    by 0x11466B: Thread::main() (thread.h:43)
==20851==    by 0x11B69F: void std::__invoke_impl<void, void (Thread::*)(), Thread*>(std::__invoke_memfun_deref, void (Thread::*&&)(), Thread*&&) (invoke.h:74)
==20851==    by 0x11B5F2: std::__invoke_result<void (Thread::*)(), Thread*>::type std::__invoke<void (Thread::*)(), Thread*>(void (Thread::*&&)(), Thread*&&) (invoke.h:96)
==20851==    by 0x11B552: void std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::_M_invoke<0ul, 1ul>(std::_Index_tuple<0ul, 1ul>) (std_thread.h:259)
==20851==    by 0x11B399: std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> >::operator()() (std_thread.h:266)
==20851==    by 0x11B259: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (Thread::*)(), Thread*> > >::_M_run() (std_thread.h:211)
==20851==    by 0x494C252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==20851==    by 0x4C37AC2: start_thread (pthread_create.c:442)
==20851==  Address 0x4f70df8 is 8 bytes inside a block of size 56 free'd
==20851==    at 0x484BB6F: operator delete(void*, unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==20851==    by 0x120311: __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::deallocate(std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >*, unsigned long) (new_allocator.h:145)
==20851==    by 0x11FFA6: std::allocator_traits<std::allocator<std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > > >::deallocate(std::allocator<std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >&, std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >*, unsigned long) (alloc_traits.h:496)
==20851==    by 0x11F9D8: std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::_M_put_node(std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >*) (stl_tree.h:565)
==20851==    by 0x11F3ED: std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::_M_drop_node(std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >*) (stl_tree.h:632)
==20851==    by 0x11E71A: std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::_M_erase(std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >*) (stl_tree.h:1891)
==20851==    by 0x128A5F: std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::clear() (stl_tree.h:1254)
==20851==    by 0x129660: std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::_M_erase_aux(std::_Rb_tree_const_iterator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::_Rb_tree_const_iterator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >) (stl_tree.h:2498)
==20851==    by 0x127122: std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::erase(unsigned char const&) (stl_tree.h:2512)
==20851==    by 0x125678: std::map<unsigned char, std::shared_ptr<PlayerState>, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::erase(unsigned char const&) (stl_map.h:1069)
==20851==    by 0x120B08: GameState::removePlayer(unsigned char) (game_state.cpp:25)
==20851==    by 0x11BCC5: GameLoop::removePlayer(unsigned char) (gameloop.cpp:75)
==20851==  Block was alloc'd at
==20851==    at 0x4849013: operator new(unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==20851==    by 0x12D547: __gnu_cxx::new_allocator<std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::allocate(unsigned long, void const*) (new_allocator.h:127)
==20851==    by 0x12C5C9: std::allocator_traits<std::allocator<std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > > >::allocate(std::allocator<std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >&, unsigned long) (alloc_traits.h:464)
==20851==    by 0x12B07C: std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::_M_get_node() (stl_tree.h:561)
==20851==    by 0x128E8A: std::_Rb_tree_node<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >* std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::_M_create_node<std::piecewise_construct_t const&, std::tuple<unsigned char const&>, std::tuple<> >(std::piecewise_construct_t const&, std::tuple<unsigned char const&>&&, std::tuple<>&&) (stl_tree.h:611)
==20851==    by 0x126F90: std::_Rb_tree_iterator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > std::_Rb_tree<unsigned char, std::pair<unsigned char const, std::shared_ptr<PlayerState> >, std::_Select1st<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::_M_emplace_hint_unique<std::piecewise_construct_t const&, std::tuple<unsigned char const&>, std::tuple<> >(std::_Rb_tree_const_iterator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > >, std::piecewise_construct_t const&, std::tuple<unsigned char const&>&&, std::tuple<>&&) (stl_tree.h:2431)
==20851==    by 0x1255DF: std::map<unsigned char, std::shared_ptr<PlayerState>, std::less<unsigned char>, std::allocator<std::pair<unsigned char const, std::shared_ptr<PlayerState> > > >::operator[](unsigned char const&) (stl_map.h:501)
==20851==    by 0x120FAF: GameState::doAction(unsigned char, unsigned char) (game_state.cpp:56)
==20851==    by 0x11BC31: GameLoop::doActionGameState(unsigned char, unsigned char) (gameloop.cpp:70)
==20851==    by 0x13A13F: Receiver::run()::{lambda()#1}::operator()() const (receiver.cpp:25)
==20851==    by 0x13A8E3: void std::__invoke_impl<void, Receiver::run()::{lambda()#1}&>(std::__invoke_other, Receiver::run()::{lambda()#1}&) (invoke.h:61)
==20851==    by 0x13A7D7: std::enable_if<is_invocable_r_v<void, Receiver::run()::{lambda()#1}&>, void>::type std::__invoke_r<void, Receiver::run()::{lambda()#1}&>(Receiver::run()::{lambda()#1}&) (invoke.h:111)
==20851== 

*/