#include "client_protocol.h"

void ClientProtocol::sendCommand(uint8_t num, bool &wasClosed) {
    this->sendUint8(this->client_identifier, wasClosed);
    this->sendUint8(num, wasClosed);
}

armor_t ClientProtocol::readArmor(bool& wasClosed){
    armor_t armor;
    armor.pos = readPosition(wasClosed);
    armor.type = recvUint8(wasClosed);
    armor.is_equipped = recvUint8(wasClosed) ? true : false;
    return armor;
}

weapon_t ClientProtocol::readWeapon(bool& wasClosed){
    weapon_t weapon;
    weapon.pos = readPosition(wasClosed);
    weapon.type = recvUint8(wasClosed);
    weapon.ammo = recvUint8(wasClosed);
    weapon.is_equipped = recvUint8(wasClosed) ? true : false;
    return weapon;
}

position_t ClientProtocol::readPosition(bool& wasClosed){
    position_t position;
    position.x = recvUint32(wasClosed);
    position.y = recvUint32(wasClosed);
    position.w = recvUint32(wasClosed);
    position.h = recvUint32(wasClosed);
    return position;
}

duck_t ClientProtocol::readDuck(bool& wasClosed){
    duck_t duck;
    duck.pos = readPosition(wasClosed);
    duck.id = recvUint8(wasClosed);
    duck.faceLeft = recvUint8(wasClosed) ? true : false;
    duck.isJumping = recvUint8(wasClosed) ? true : false;
    duck.isDucking = recvUint8(wasClosed) ? true : false;
    duck.isFalling = recvUint8(wasClosed) ? true : false;
    duck.isFlaping = recvUint8(wasClosed) ? true : false;
    duck.health = recvUint8(wasClosed);
    duck.isAlive = recvUint8(wasClosed) ? true : false;
    duck.score = recvUint8(wasClosed);
    duck.color = recvUint8(wasClosed);
    duck.equipped_weapon = readWeapon(wasClosed);
    duck.equipped_armor = readArmor(wasClosed);
    return duck;
}

void ClientProtocol::readDucks(duck_t ducks[MAX_DUCKS], uint8_t numDucks, bool &wasClosed) {
    for (int i = 0; i < numDucks; i++) {
        ducks[i] = readDuck(wasClosed);
    }
}

void ClientProtocol::readPlatforms(platform_t platforms[MAX_PLATFORMS], uint8_t numPlatforms, bool &wasClosed) {
    for (uint8_t i = 0; i < numPlatforms; i++) {
        platforms[i].platform = readPosition(wasClosed);
    }
}

void ClientProtocol::readSpawnPlaces(spawn_place_t spawn_places[MAX_SPAWN_PLACES], uint8_t numSpawnPlaces, bool &wasClosed) {
    for (uint8_t i = 0; i < numSpawnPlaces; i++) {
        spawn_places[i].pos = readPosition(wasClosed);
        spawn_places[i].is_active = recvUint8(wasClosed);
        spawn_places[i].weapon = readWeapon(wasClosed);
        spawn_places[i].armor = readArmor(wasClosed);
    }
}

void ClientProtocol::readBoxes(box_t boxes[MAX_BOXES], uint8_t numBoxes, bool &wasClosed) {
    for (uint8_t i = 0; i < numBoxes; i++) {
        boxes[i].pos = readPosition(wasClosed);
        boxes[i].is_explosive = recvUint8(wasClosed);
        boxes[i].is_destroyed = recvUint8(wasClosed);
        boxes[i].weapon = readWeapon(wasClosed);
        boxes[i].armor = readArmor(wasClosed);
    }
}

void ClientProtocol::readProjectiles(projectile_t projectiles[MAX_PROJECTILES], uint8_t numProjectiles, bool &wasClosed) {
    for (uint8_t i = 0; i < numProjectiles; i++) {
        projectiles[i].pos = readPosition(wasClosed);
        projectiles[i].type = recvUint8(wasClosed);
        projectiles[i].is_active = recvUint8(wasClosed);
    }
}

void ClientProtocol::readLevel(level_t& level, bool &wasClosed) {
    level.num_ducks = recvUint8(wasClosed);
    readDucks(level.ducks, level.num_ducks, wasClosed);
    level.num_platforms = recvUint8(wasClosed);
    readPlatforms(level.platforms, level.num_platforms, wasClosed);
    level.num_spawn_places = recvUint8(wasClosed);
    readSpawnPlaces(level.spawn_places, level.num_spawn_places, wasClosed);
    level.num_boxes = recvUint8(wasClosed);
    readBoxes(level.boxes, level.num_boxes, wasClosed);
    level.num_projectiles = recvUint8(wasClosed);
    readProjectiles(level.projectiles, level.num_projectiles, wasClosed);
}

game_state_t ClientProtocol::readFromServer(bool &wasClosed) {
    game_state_t game_state;
    readLevel(game_state.level, wasClosed);
    game_state.current_level = recvUint8(wasClosed);    
    game_state.round = recvUint8(wasClosed);    
    game_state.winning_score = recvUint8(wasClosed);    
    std::cout << "winning score: " << static_cast<int>(game_state.winning_score) << std::endl; // verifico si se envia bien, deberìa ser 10
    return game_state;
}


ClientProtocol::ClientProtocol(Socket&& socket, uint8_t client_id)
        : Protocol(std::move(socket)),  
          client_identifier(client_id) {
            std::cout << "Id inicializando en: " << static_cast<int>(client_identifier) << std::endl;
          }  