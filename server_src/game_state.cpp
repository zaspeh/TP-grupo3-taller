#include "game_state.h"
#include <cmath>
// Constructor
GameState::GameState() : level(0) {
    currentLevel = level.getLevel();
    std::cout << "Instancio el nivel" << std::endl;
    players = std::map<uint8_t, std::shared_ptr<PlayerState>>();
    projectilePhysics.resize(MAX_PROJECTILES);
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

game_state_t GameState::doAction(uint8_t id, uint8_t action) {
    std::lock_guard<std::mutex> lock(mtx);
    auto player = players[id];
    weapon_t weaponST;
    armor_t armorST;
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
            armorST = getArmorPosition(player->getPosition(), player->hasHelmetEquipped(), player->hasArmorEquipped());

            if (armorST.type != NULL_ARMOR) { // si encontre una armadura
                if (armorST.type == HELMET_ARMOR)
                    player->setHelmetEquipped(armorST);
                if (armorST.type == CHESTPLATE_ARMOR)
                    player->setArmorEquipped(armorST);

                break;
            }

            weaponST = getWeaponPosition(player->getPosition());
            if (weaponST.type != NULL_WEAPON){ 
                weapon = createWeapon(weaponST.type);
                checkIfDropWeapon(player->pickWeapon(weapon));
            } else { 
                checkIfDropWeapon(player->dropWeapon());
            }
            break;

        case SHOOT:
            if(player->getWeapon() == nullptr || player->getWeaponType() == NULL_WEAPON) 
                break;
            player->shoot();
            createProjectile(player->getWeaponType(), player->getPosition(), player->getFacingDirection());
            break;
        case LOOK_UP:
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
    return state;
}

void GameState::updateWeaponsPhysics(float deltaTime) {
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
}

void GameState::createProjectile(uint8_t weaponType, position_t origin, bool facingLeft) {

    bool projectileLoaded = false;
    int index = 0;
    for (int i = 0; i < state.level.num_projectiles && i < MAX_PROJECTILES; i++) {
        if (!state.level.projectiles[i].is_active) {
            state.level.projectiles[i].type = weaponType;
            state.level.projectiles[i].pos = origin;
            state.level.projectiles[i].is_active = true;
            projectileLoaded = true;
            index = i;
            break;
        }
    }
    if(!projectileLoaded){
        projectile_t newProjectile;
        newProjectile.pos = origin;
        newProjectile.type = weaponType;
        newProjectile.is_active = true;
        state.level.projectiles[state.level.num_projectiles] = newProjectile;
        index = state.level.num_projectiles;
    }
    
    float initialVelocity = 1000.0f;
    float initialAngle = facingLeft ? M_PI : 0.0f; // por si tiene que tener caìda
    //if (weaponType == GRENADE_WEAPON || weaponType == BANANA_WEAPON) 
        //initialAngle = facingLeft ? M_PI : 1.0f; // por si tiene que tener caìda

    projectilePhysics[index].initProjectile(initialVelocity, initialAngle, origin.x);
    
    if(index == state.level.num_projectiles)
        state.level.num_projectiles++;
}

void GameState::updateProjectilsPhysics(float deltaTime) {
    for (size_t i = 0; i < state.level.num_projectiles; i++) {
        uint8_t maxDistance = checkWeaponDistance(state.level.projectiles[i].type);
        state.level.projectiles[i].is_active = projectilePhysics[i].updatePosition(state.level.projectiles[i] ,level, deltaTime, maxDistance, this);
        // if false -> lo elimino asì no aparece otra vez.
    }
}

game_state_t GameState::updatePlayers(float deltaTime) {
    deltaTime = std::min(deltaTime, 0.033f); 
    
    for (auto& [id, player] : players) {
        player->updatePosition(deltaTime, state.level.platforms, state.level.num_platforms);
        updateState(id, player); 
    }

    
    updateWeaponsPhysics(deltaTime);
    updateProjectilsPhysics(deltaTime);
    updateBoxes();

    return state;
}


void GameState::updateBoxes() {

    int i = 0;
    std::cout << "Cantidad de cajas: " << level.getBoxes().size() << std::endl;
    for (auto box : level.getBoxes()) {
        box->setBoxState(state.level.boxes[i]);
        box_t boxState = box->getBoxState();
        armor_t armorState = box->getArmorState();
        weapon_t weaponState = box->getWeaponState();
        std::cout << "TIPO DE ARMA : " << static_cast<int>(weaponState.type) << std::endl;
        if(boxState.health == 0 && !box->isBroken()){ 
            if (weaponState.type != NULL_WEAPON) {
                state.level.dropped_weapons[state.level.num_dropped_weapons] = weaponState;
                state.level.dropped_weapons[state.level.num_dropped_weapons].pos = {boxState.pos.x-10, boxState.pos.y-20};
                state.level.num_dropped_weapons++;
            } else if (armorState.type != NULL_ARMOR) {
                state.level.dropped_armors[state.level.num_dropped_armors] = armorState;
                state.level.dropped_armors[state.level.num_dropped_armors].pos = {boxState.pos.x-10, boxState.pos.y-20};
                state.level.num_dropped_armors++;
            } //else { // la caja es explosiva  (?) hacer daño a los jugadores al redededor
            box->breakBox();
        }       
        i++;
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
    };
    
    const int pickupRadius = 20; // Radio de recogida del arma
    
    for (int i = 4; i < state.level.num_spawn_places; i++) {
        float dx = state.level.spawn_places[i].pos.x - position.x;
        float dy = state.level.spawn_places[i].pos.y - position.y;
        float distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
        
        if (distance <= pickupRadius && state.level.spawn_places[i].weapon.type != NULL_WEAPON) {
            weapon_t pickedWeapon = state.level.spawn_places[i].weapon;
            weapon = pickedWeapon;
            state.level.spawn_places[i].weapon.type = NULL_WEAPON; // Arma recogida, remover del spawn
            break;
        }
    }

    for (int i = 0; i < state.level.num_dropped_weapons; i++) {
        float dx = state.level.dropped_weapons[i].pos.x - position.x;
        float dy = state.level.dropped_weapons[i].pos.y - position.y;
        float distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
        
        if (distance <= pickupRadius && state.level.dropped_weapons[i].type != NULL_WEAPON) {
            weapon_t pickedWeapon = state.level.dropped_weapons[i];
            weapon = pickedWeapon;
            state.level.dropped_weapons[i].type = NULL_WEAPON; 
            break;
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
    
    for (int i = 4; i < state.level.num_spawn_places; i++) {
        float dx = state.level.spawn_places[i].pos.x - position.x;
        float dy = state.level.spawn_places[i].pos.y - position.y;
        float distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
        
        if (distance <= pickupRadius && state.level.spawn_places[i].armor.type != NULL_ARMOR) {
            
            armor_t pickedarmor = state.level.spawn_places[i].armor;
            if(pickedarmor.type == HELMET_ARMOR && !helmetEquipped) {   
                armor = pickedarmor;
                state.level.spawn_places[i].armor.type = NULL_ARMOR; 
                return armor;
            }

            if(pickedarmor.type == CHESTPLATE_ARMOR && !armorEquipped) {
                armor = pickedarmor;
                state.level.spawn_places[i].armor.type = NULL_ARMOR; 
                return armor;
            }
        }
    }

    for (int i = 4; i < state.level.num_dropped_armors; i++) {
        float dx = state.level.dropped_armors[i].pos.x - position.x;
        float dy = state.level.dropped_armors[i].pos.y - position.y;
        float distance = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
        
        if (distance <= pickupRadius && state.level.dropped_armors[i].type != NULL_ARMOR) {
            
            armor_t pickedarmor = state.level.dropped_armors[i];
            if(pickedarmor.type == HELMET_ARMOR && !helmetEquipped) {   
                armor = pickedarmor;
                state.level.dropped_armors[i].type = NULL_ARMOR; 
                return armor;
            }

            if(pickedarmor.type == CHESTPLATE_ARMOR && !armorEquipped) {
                armor = pickedarmor;
                state.level.dropped_armors[i].type = NULL_ARMOR; 
                return armor;
            }
        }
    }

    return armor;
}

Weapon* GameState::createWeapon(uint8_t weaponType) {
    std::cout << "Arma tomada\n";
    Weapon* newWeapon;
    try { 
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
                break;
            case AK_47_WEAPON:
                newWeapon = new AK47();
                break;
            default:
                std::cerr << "Arma no creada\n";
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