#include "game_state.h"

// Constructor
GameState::GameState() : level(0) {
    currentLevel = level.getLevel();
    players = std::map<uint8_t, std::shared_ptr<PlayerState>>();
    
    state = {
        currentLevel,
        0,
        0,
        10
    };
}

std::shared_ptr<PlayerState> GameState::getPlayer(uint8_t id) {
    std::lock_guard<std::mutex> lock(mtx);
    return players.count(id) ? players[id] : nullptr;
}

void GameState::removePlayer(uint8_t id) {
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Pato eliminado: " << static_cast<int>(id) << "\n";
    if (players.count(id)) {
        players.erase(id);

        if (state.level.num_ducks > 0) {
            state.level.num_ducks--;
        }

        state.level.ducks[id].isAlive = false;
    }
}

void GameState::updateState(uint8_t id, std::shared_ptr<PlayerState> player) {
    state.level.ducks[id] = player->getState();
}

std::shared_ptr<PlayerState> GameState::connectPlayer(uint8_t id) {
    std::cout << "Nuevo jugador: " << static_cast<int>(id) << std::endl;
    position_t pos = level.getSpawnPosition();
    players[id] = std::make_shared<PlayerState>(id, pos.x, pos.y);  
    state.level.ducks[id] = players[id]->getState();
    state.level.num_ducks++;  

    std::cout << "Cantidad de jugadores: "<< static_cast<int>(state.level.num_ducks) << std::endl;
    return players[id];
}

std::map<uint8_t, std::shared_ptr<PlayerState>> GameState::getPlayers() {
    std::lock_guard<std::mutex> lock(mtx);
    return players;
}

game_state_t GameState::doAction(uint8_t id, uint8_t action) {
    //std::cout << "A punto de realizar una acción\n";
    std::lock_guard<std::mutex> lock(mtx);
    //std::cout << "Realizando acción\n";
    auto player = players[id];
    weapon_t weaponST;
    Weapon* weapon = nullptr;
    switch(action) {
        case MOVE_LEFT:
            player->move(-10, 0, state.level.platforms, state.level.num_platforms);
            player->setFacingDirection(1);
            break;
        case MOVE_RIGHT:
            std::cout << "Moving right" << std::endl;
            player->move(10, 0, state.level.platforms, state.level.num_platforms);
            player->setFacingDirection(0);
            break;
        case JUMP:
            player->jump();
            break;
        case TAKE_WEAPON:
            std::cout << "Arma tomada\n";
            weaponST = getWeaponPosition(player->getPosition());
            if (weaponST.type == 0){ 
                std::cout << "Dropping weapon\n";
                player->dropWeapon();
            } else {
                std::cout << "Eligiendo arma: " << static_cast<int>(weaponST.type) << std::endl;
                weapon = createWeapon(weaponST.type);
                player->pickWeapon(weapon);
            }
            break;
        case SHOOT:
            player->shoot();
            break;
        case LOOK_UP:
            // Implementar lógica para mirar hacia arriba
            break;
        case FLOOR:
            player->setCrouched(!player->isCrouched());
            break;
        case NEW_CLIENT:
            std::cout << "Agregando nuevo cliente\n";
            player = connectPlayer(id);
            break;
        default:
            std::cout << "Unknown action: " << action << std::endl;
            break;
    }

    updateState(id, player);
    //std::cout << "Accion realizada\n";
    return state;
}

game_state_t GameState::updatePlayers(float deltaTime) {
    deltaTime = std::min(deltaTime, 0.033f); // Máximo ~30 FPS
    
    for (auto& [id, player] : players) {
        player->updatePosition(deltaTime, state.level.platforms, state.level.num_platforms);
        updateState(id, player); 
    }
    return state;
}



weapon_t GameState::getWeaponPosition(position_t position) {
    weapon_t weapon = {
        {0, 0},
        NULL_WEAPON,
    };
    for (int i = 4; i < state.level.num_spawn_places; i++) {
        if (state.level.spawn_places[i].pos.x == position.x && state.level.spawn_places[i].pos.y == position.y) {
            weapon = state.level.spawn_places[i].weapon;
            state.level.spawn_places[i].weapon.type = NULL_WEAPON;
            break;
        }
    }

    return weapon;
}

/*
#define GRENADE_WEAPON 0x1
#define BANANA_WEAPON 0x2
#define PEWPEWLASER_WEAPON 0x3
#define LASERRIFLE_WEAPON 0x4
*/
Weapon* GameState::createWeapon(uint8_t weaponType) {
    std::cout << static_cast<int> (weaponType) << std::endl;
    Weapon* newWeapon;
    switch (weaponType) {
        case GRENADE_WEAPON:
            newWeapon = new Grenade();
            break;
        case BANANA_WEAPON:
            newWeapon = new Banana();
            break;
        case PEWPEWLASER_WEAPON:
            newWeapon = new PewPewLaser();
            break;
        case LASERRIFLE_WEAPON:
            newWeapon = new LaserRifle();
            break;
        case DARTGUN_WEAPON:
            newWeapon = new Dartgun();
        case AK_47_WEAPON:
            newWeapon = new AK47();
        case NULL_WEAPON:
            newWeapon = new Dartgun();
        default:
            std::cerr << "Arma no creada\n";
            newWeapon = nullptr;
            break;
    }
    return newWeapon;
}