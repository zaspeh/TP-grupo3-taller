#include "game_state.h"
#include "server.h"
#include <cmath>

GameState::GameState(Server& server) : level(), server(server) {
    players = std::map<uint8_t, std::shared_ptr<PlayerState>>();
    projectilePhysics.resize(MAX_PROJECTILES);
    
    state = {
        level.getLevel(),
        0,
        0,
        5
    };
}

std::shared_ptr<PlayerState> GameState::getPlayer(uint8_t id) {
    std::lock_guard<std::mutex> lock(mtx);
    return players.count(id) ? players[id] : nullptr;
}

void GameState::removePlayer(uint8_t id) {
    if (players.count(id)) {
        players.erase(id);
        if (state.level.num_ducks > 0) {
            state.level.num_ducks--;
        }
        state.level.ducks[id].isAlive = false;
    }
    
    if (players.empty()) { 
        server.stop();
    }
}

void GameState::updateState(uint8_t id, std::shared_ptr<PlayerState> player) {
    state.level.ducks[id] = player->getState();
}


std::shared_ptr<PlayerState> GameState::connectPlayer(uint8_t id) {
    position_t pos = level.getSpawnPosition();
    players[id] = std::make_shared<PlayerState>(id, pos.x, pos.y);  
    state.level.ducks[id] = players[id]->getState();
    state.level.num_ducks++;  

    return players[id];
}

std::map<uint8_t, std::shared_ptr<PlayerState>> GameState::getPlayers() {
    std::lock_guard<std::mutex> lock(mtx);
    return players;
}

bool GameState::chosedAWeapon(uint8_t id,uint8_t action){
    if(!pickAnyWeapon) return false;
    pickAnyWeapon = false;
    weapon_t weaponState = {{0, 0}, action, ammoForWeapons[action]};
    Weapon *weapon = createWeapon(weaponState);
    if(weapon == nullptr) return false;
    checkIfDropWeapon(players[id]->pickWeapon(weapon));
    return true;
}

game_state_t GameState::doAction(uint8_t id, uint8_t action) {
    std::lock_guard<std::mutex> lock(mtx);
    auto player = players[id];
    if (player && !player->isAlive()) {
        return state;
    }
    if (matchFinished && action == RESTART_MATCH) {
        for (auto& [id, player] : players) {
            state.level.ducks[id].score = 0;
            player->resetPlayer(state.level.ducks[id], player->getPosition().x, player->getPosition().y);
            state.level.ducks[id] = player->getState(); 
        }
        matchFinished = false;
        changeLevel();
    }

    weapon_t weaponST;
    armor_t armorST;
    Weapon* weapon = nullptr;

    if(!chosedAWeapon(id, action)){
        std::cout << "Accion a realizarse: " << static_cast<int>(action) << std::endl;
        switch(action) {
            case MOVE_LEFT:
                player->move(-10, 0, state.level.platforms, state.level.num_platforms);
                player->setFacingDirection(1);
                break;
            case MOVE_RIGHT:
                player->move(10, 0, state.level.platforms, state.level.num_platforms);
                player->setFacingDirection(0);
                break;
            case JUMP:
                player->jump();
                break;
            case TAKE_WEAPON:
                armorST = getArmorPosition(player->getPosition(), player->hasHelmetEquipped(), player->hasArmorEquipped());

                if (armorST.type != NULL_ARMOR) { 
                    if (armorST.type == HELMET_ARMOR)
                        player->setHelmetEquipped(armorST);
                    if (armorST.type == CHESTPLATE_ARMOR)
                        player->setArmorEquipped(armorST);
                    break;
                }

                weaponST = getWeaponPosition(player->getPosition());
                if (weaponST.type != NULL_WEAPON){ 
                    weapon = createWeapon(weaponST);
                    checkIfDropWeapon(player->pickWeapon(weapon));
                } else { 
                    checkIfDropWeapon(player->dropWeapon());
                }
                break;

            case SHOOT:
                try {
                    if(player->getWeapon() == nullptr || player->getWeaponType() == NULL_WEAPON) 
                        break;
                    if(player->shoot())
                        createProjectile(player->getWeaponType(), player->getPosition(), player->getFacingDirection());
                    break;
                } catch (const std::exception& e) {
                    std::cerr << "Error al disparar: " << e.what() << std::endl;
                }
            case LOOK_UP:
                break;
            case FLOOR:
                player->setCrouched(!player->isCrouched());
                break;
            case NEW_CLIENT:
                std::cout << "Agregando nuevo cliente\n";
                player = connectPlayer(id);
                std::cout << "Agregado\n";
                break;
            case INFINIT_AMMO:
                std::cout << "Infinite ammo: " << player->isInfiniteAmmo() << std::endl;
                player->setInfiniteAmmo(!player->isInfiniteAmmo());
                break;
            case PICK_ANY_WEAPON:
                pickAnyWeapon = !pickAnyWeapon;
                break;
            case CHESTPLATE_ARMOR:
                armorST = { {0,0} , CHESTPLATE_ARMOR };
                player->setArmorEquipped(armorST);
                break;
            case HELMET_ARMOR:
                armorST = { {0,0} , HELMET_ARMOR };
                player->setHelmetEquipped(armorST);
                break;
            case LEAVE_MATCH:
                removePlayer(id);
                break;
            default:
                std::cout << "Unknown action: " << action << std::endl;
                break;
        }
    }

    updateState(id, player);
    std::cout << "Acomodo el estado de los jugadores" << std::endl;
    return state;
}


void GameState::createProjectile(uint8_t weaponType, position_t origin, bool facingLeft) {

    bool projectileLoaded = false;
    int index = 0;
    for (int i = 0; i < state.level.num_projectiles && i < MAX_PROJECTILES; i++) {
        if (!state.level.projectiles[i].is_active) {
            state.level.projectiles[i].type = weaponType;
            state.level.projectiles[i].pos = origin;
            if (facingLeft) {
                state.level.projectiles[i].pos.x -= 20;
            } else {
                state.level.projectiles[i].pos.x += 40;
            }
            state.level.projectiles[i].pos.y += 10;
            state.level.projectiles[i].is_active = true;
            projectileLoaded = true;
            index = i;
            break;
        }
    }
    if(!projectileLoaded){
        projectile_t newProjectile;
        newProjectile.pos = origin;
        if (facingLeft) {
            newProjectile.pos.x -= 20;
        } else {
            newProjectile.pos.x += 40;
        }
        newProjectile.pos.y += 10;
        newProjectile.type = weaponType;
        newProjectile.is_active = true;
        state.level.projectiles[state.level.num_projectiles] = newProjectile;
        index = state.level.num_projectiles;
    }
    
    float initialVelocity = 2000.0f;
    float initialAngle;
    float gravity = 980.0f;
    if (weaponType == GRENADE_WEAPON || weaponType == BANANA_WEAPON) {
        initialVelocity = 1000.0f;  
        gravity = 2500.0f;
        initialAngle = facingLeft ? M_PI - 5.5f : 5.5f; 
    } else {
        initialAngle = facingLeft ? M_PI : 0.0f;
    }

    projectilePhysics[index].initProjectile(initialVelocity, initialAngle, origin.x, gravity);
    

    if(index == state.level.num_projectiles)
        state.level.num_projectiles++;
}

bool GameState::explotionInPosition(position_t position) {
    for (size_t i = 0; i < state.level.num_explosions; i++) {
        if (state.level.explosions[i].x == position.x && state.level.explosions[i].y == position.y){
            return true;
        }
    }
    return false;
}

void GameState::updateProjectilsPhysics(float deltaTime) {
    for (size_t i = 0; i < state.level.num_projectiles; i++) {
        uint8_t maxDistance = checkWeaponDistance(state.level.projectiles[i].type);
        state.level.projectiles[i].is_active = projectilePhysics[i].updatePosition(state.level.projectiles[i], state.level, deltaTime, maxDistance, players);
        
        if (!state.level.projectiles[i].is_active && state.level.projectiles[i].type == GRENADE_WEAPON && !explotionInPosition(state.level.projectiles[i].pos)) {
            state.level.explosions[state.level.num_explosions++] = state.level.projectiles[i].pos;
        }
    }
}

void GameState::updateWeaponsPhysics(float deltaTime) {
    try {
        for (size_t i = 0; i < weaponsInAir.size(); ) {
            int droppedIndex = -1;
            for (int j = 0; j < state.level.num_dropped_weapons; j++) {
                if (state.level.dropped_weapons[j].type == weaponsInAir[i].type &&
                    state.level.dropped_weapons[j].pos.x == weaponsInAir[i].pos.x &&
                    state.level.dropped_weapons[j].pos.y == weaponsInAir[i].pos.y) {
                    droppedIndex = j;
                    break;
                }
            }
            
            if (fallingWeapons[i].updatePosition(weaponsInAir[i], deltaTime, state.level.platforms, state.level.num_platforms)) {
                if (droppedIndex != -1) {
                    state.level.dropped_weapons[droppedIndex].pos = weaponsInAir[i].pos;
                }
                
                weaponsInAir.erase(weaponsInAir.begin() + i);
                fallingWeapons.erase(fallingWeapons.begin() + i);
            } else {
                if (droppedIndex != -1) {
                    state.level.dropped_weapons[droppedIndex].pos = weaponsInAir[i].pos;
                }
                i++; 
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error updating weapons physics: " << e.what() << std::endl;
    }
}

void GameState::updateArmorsPhysics(float deltaTime) {
    for (size_t i = 0; i < armorsInAir.size();) {
        int droppedIndex = -1;
        for (int j = 0; j < state.level.num_dropped_armors; j++) {
            if (state.level.dropped_armors[j].type == armorsInAir[i].type &&
                state.level.dropped_armors[j].pos.x == armorsInAir[i].pos.x &&
                state.level.dropped_armors[j].pos.y == armorsInAir[i].pos.y) {
                droppedIndex = j;
                break;
            }
        }

        if (fallingArmors[i].updatePosition(armorsInAir[i], deltaTime, state.level.platforms, state.level.num_platforms)) {
            if (droppedIndex != -1) {
                state.level.dropped_armors[droppedIndex].pos = armorsInAir[i].pos;
            }
            armorsInAir.erase(armorsInAir.begin() + i);
            fallingArmors.erase(fallingArmors.begin() + i);
        } else {
            if (droppedIndex != -1) {
                state.level.dropped_armors[droppedIndex].pos = armorsInAir[i].pos;
            }
            i++;
        }
    }
}


game_state_t GameState::updatePlayers(float deltaTime) {
    deltaTime = std::min(deltaTime, 0.033f); 
    try {
        for (auto& [id, player] : players) {
            player->updatePosition(deltaTime, state.level.platforms, state.level.num_platforms, state.level.explosions, state.level.num_explosions);
            player->updateWeapon(deltaTime, state.level);
            updateState(id, player); 
        }

        updateBoxes();
        updateWeaponsPhysics(deltaTime);
        updateArmorsPhysics(deltaTime);
        updateProjectilsPhysics(deltaTime);
        updateSpawns(deltaTime);
        checkIfSomeoneWin();
    } catch (const std::exception& e) {
        std::cerr << "Error updating players: " << e.what() << std::endl;
    }
    return state;
}

void GameState::updateSpawns(float deltaTime) {
    std::vector<std::shared_ptr<Spawn>> spawns = level.getSpawns();
    for (size_t i = 0; i < state.level.num_spawn_places; i++) {

        if (!spawns[i]->hasSomething && !spawns[i]->duckCanSpawn) {
            spawns[i]->respawnTimer -= deltaTime;

            if (spawns[i]->respawnTimer <= 0.0f) {
                spawns[i]->hasSomething = true;
                spawns[i]->respawnTimer = 0.0f;
                position_t pos = state.level.spawn_places[i].pos;
                state.level.spawn_places[i] = level.getRandomSpawnPlace(pos.x, pos.y);
            }
        }
    }
}

void GameState::finishMatch(uint8_t id) {
    std::cout << "EL JUGADOR : " << static_cast<int>(id) << " HA GANADO" << std::endl;
    matchFinished = true;
    
    std::map<uint8_t, duck_t> currentDucks;
    for (auto& [id, player] : players) {
        currentDucks[id] = state.level.ducks[id];
    }

    level.initWinningLevel();
    state.level = level.getLevel();

    for (auto& [id, player] : players) {
        position_t pos = level.getSpawnPosition();
        player->resetPlayer(currentDucks[id], pos.x, pos.y);
        state.level.ducks[id] = player->getState();
    }
        
    state.level.num_ducks = players.size();
}

void GameState::changeLevel() {
    std::map<uint8_t, duck_t> currentDucks;
    for (auto& [id, player] : players) {
        if(state.level.ducks[id].isAlive && !matchFinished) state.level.ducks[id].score += 1;
        currentDucks[id] = state.level.ducks[id];
    }

    level.createNewLevel();
    state.level = level.getLevel();

    for (auto& [id, player] : players) {
        position_t pos = level.getSpawnPosition();
        player->resetPlayer(currentDucks[id], pos.x, pos.y);
        state.level.ducks[id] = player->getState(); 
    }
        
    state.level.num_ducks = players.size();
}

void GameState::checkIfSomeoneWin() {
    int aliveDucks = 0;
    for (auto& [id, player] : players) {
        if (player->isAlive()) 
            aliveDucks++;
        
        if (state.level.ducks[id].score >= state.winning_score && !matchFinished) {
            finishMatch(id);
            return;
        }
    }   

    if (aliveDucks == 1 && players.size() > 1) {
       changeLevel();
    }
}

void GameState::updateBoxes() {
    int i = 0;
    try {
        for (auto box : level.getBoxes()) {
            box->setBoxState(state.level.boxes[i]);
            box_t boxState = box->getBoxState();
            armor_t armorState = box->getArmorState();
            weapon_t weaponState = box->getWeaponState();

            if (boxState.health == 0 && !box->isBroken()) {
                
                if (weaponState.type != NULL_WEAPON) {
                    weaponState.pos = {boxState.pos.x - 10, boxState.pos.y - 20};
                    checkIfDropWeapon(weaponState);
                } else if (armorState.type != NULL_ARMOR) {
                    armorState.pos = {boxState.pos.x - 10, boxState.pos.y - 20};
                    checkIfDropArmor(armorState);
                }

                box->breakBox();
            }
            i++;
        }
    } catch (...) { // ?
        std::cerr << "Error inesperado en updateBoxes" << std::endl;
    }
}


uint8_t GameState::checkWeaponDistance(uint8_t weaponType) {
    switch (weaponType) {
        case GRENADE_WEAPON:
            return GRENADE_DISTANCE;
        case BANANA_WEAPON:
            return BANANA_DISTANCE;
        case PEWPEWLASER_WEAPON:
            return PEWPEWLASER_DISTANCE;
        case LASERRIFLE_WEAPON:
            return LASERRIFLE_DISTANCE;
        case DARTGUN_WEAPON:
            return DARTGUN_DISTANCE;
        case AK_47_WEAPON:
            return AK_47_DISTANCE;
        case COWBOY_WEAPON:
            return COWBOY_DISTANCE;
        case MAGNUM_WEAPON:
            return MAGNUM_DISTANCE;
        case SHOTGUN_WEAPON:
            return SHOTGUN_DISTANCE;
        case SNIPER_WEAPON:
            return SNIPER_DISTANCE;
        default:
            return 0;
    }   
}

weapon_t GameState::getWeaponPosition(position_t position) { // SE PUEDE MODULARIZAR
    weapon_t weapon = {
        {0, 0},
        NULL_WEAPON,
        0
    };
    
    const int pickupRadius = 20;
    std::vector<std::shared_ptr<Spawn>> spawns = level.getSpawns();

    for (int i = 4; i < state.level.num_spawn_places; i++) {
        float dx = state.level.spawn_places[i].pos.x - position.x;
        float dy = state.level.spawn_places[i].pos.y - position.y;
        float distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
        
        if (distance <= pickupRadius && state.level.spawn_places[i].weapon.type != NULL_WEAPON) {
            weapon = state.level.spawn_places[i].weapon;
            state.level.spawn_places[i].weapon.type = NULL_WEAPON;
            spawns[i]->hasSomething = false;
            spawns[i]->respawnTimer = 5.0f;
            return weapon;
        }
    }

    for (int i = 0; i < state.level.num_dropped_weapons; i++) {
        float dx = state.level.dropped_weapons[i].pos.x - position.x;
        float dy = state.level.dropped_weapons[i].pos.y - position.y;
        float distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
        
        if (distance <= pickupRadius && state.level.dropped_weapons[i].type != NULL_WEAPON) {
            weapon = state.level.dropped_weapons[i];
            state.level.dropped_weapons[i].type = NULL_WEAPON;
            spawns[i]->hasSomething = false;
            spawns[i]->respawnTimer = 5.0f;
            return weapon;
        }
    }

    return weapon;
}

armor_t GameState::getArmorPosition(position_t position, bool helmetEquipped, bool armorEquipped) {
    armor_t armor = {
        {0, 0},
        NULL_ARMOR,
    };
    
    const int pickupRadius = 20; 
    std::vector<std::shared_ptr<Spawn>> spawns = level.getSpawns();

    for (int i = 4; i < state.level.num_spawn_places; i++) {
        float dx = state.level.spawn_places[i].pos.x - position.x;
        float dy = state.level.spawn_places[i].pos.y - position.y;
        float distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
        
        if (distance <= pickupRadius && state.level.spawn_places[i].armor.type != NULL_ARMOR) {
            
            armor_t pickedarmor = state.level.spawn_places[i].armor;
            if(pickedarmor.type == HELMET_ARMOR && !helmetEquipped) {   
                armor = pickedarmor;
                state.level.spawn_places[i].armor.type = NULL_ARMOR; 

                spawns[i]->hasSomething = false;
                spawns[i]->respawnTimer = 5.0f;
                return armor;
            }

            if(pickedarmor.type == CHESTPLATE_ARMOR && !armorEquipped) {
                armor = pickedarmor;
                state.level.spawn_places[i].armor.type = NULL_ARMOR; 
                spawns[i]->hasSomething = false;
                spawns[i]->respawnTimer = 5.0f;
                return armor;
            }
        }
    }

    for (int i = 0; i < state.level.num_dropped_armors; i++) {
        float dx = state.level.dropped_armors[i].pos.x - position.x;
        float dy = state.level.dropped_armors[i].pos.y - position.y;
        float distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
        
        if (distance <= pickupRadius && state.level.dropped_armors[i].type != NULL_ARMOR) {
            armor_t pickedarmor = state.level.dropped_armors[i];
            if(pickedarmor.type == HELMET_ARMOR && !helmetEquipped) {   
                armor = pickedarmor;
                state.level.dropped_armors[i].type = NULL_ARMOR; 
                spawns[i]->hasSomething = false;
                spawns[i]->respawnTimer = 5.0f;
                return armor;
            }

            if(pickedarmor.type == CHESTPLATE_ARMOR && !armorEquipped) {
                armor = pickedarmor;
                state.level.dropped_armors[i].type = NULL_ARMOR; 

                spawns[i]->hasSomething = false;
                spawns[i]->respawnTimer = 5.0f;
                return armor;
            }
        }
    }

    return armor;
}

Weapon* GameState::createWeapon(weapon_t weaponState) {
    Weapon* newWeapon;
    try { 
        switch (weaponState.type) {
            case GRENADE_WEAPON:
                newWeapon = new Grenade(weaponState);
                break;
            case BANANA_WEAPON:
                newWeapon = new Banana(weaponState);
                break;
            case PEWPEWLASER_WEAPON:
                newWeapon = new PewPewLaser(weaponState);
                break;
            case LASERRIFLE_WEAPON:
                newWeapon = new LaserRifle(weaponState);
                break;
            case DARTGUN_WEAPON:
                newWeapon = new Dartgun(weaponState);
                break;
            case AK_47_WEAPON:
                newWeapon = new AK47(weaponState);
                break;
            case COWBOY_WEAPON:
                newWeapon = new CowBoyPistol(weaponState);
                break;
            case MAGNUM_WEAPON:
                newWeapon = new Magnum(weaponState);
                break;
            case SHOTGUN_WEAPON:
                newWeapon = new Shotgun(weaponState);
                break;
            case SNIPER_WEAPON:
                newWeapon = new Sniper(weaponState);
                break;
            default:
                newWeapon = nullptr;
                break;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return newWeapon;
}

void GameState::checkIfDropWeapon(weapon_t droppedWeapon) {
    if (droppedWeapon.type != NULL_WEAPON) {
        state.level.dropped_weapons[state.level.num_dropped_weapons] = droppedWeapon;
        state.level.num_dropped_weapons++;
        
        weaponsInAir.push_back(droppedWeapon);
        fallingWeapons.emplace_back();
    }
}

void GameState::checkIfDropArmor(armor_t droppedArmor) {
    if (droppedArmor.type != NULL_ARMOR) {
        state.level.dropped_armors[state.level.num_dropped_armors] = droppedArmor;
        state.level.num_dropped_armors++;
        armorsInAir.push_back(droppedArmor);
        fallingArmors.emplace_back();
    }
}
