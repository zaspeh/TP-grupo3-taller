#include "serverprotocol.h"

ServerProtocol::ServerProtocol(Socket socket) : Protocol(std::move(socket)) {}

std::vector<uint8_t> ServerProtocol::recvCommand(bool &wasClosed){
    std::vector<uint8_t> buffer(2);
    buffer[0] = recvUint8(wasClosed);
    buffer[1] = recvUint8(wasClosed);
    return buffer;
}

void ServerProtocol::sendGameState(game_state_t& game, bool &wasClosed) {
    //send
    /*
    game_state_t ClientProtocol::readFromServer(bool &wasClosed) {
        game_state_t game_state;
        readLevel(game_state.level, wasClosed);
        game_state.current_level = recvUint8(wasClosed);    
        game_state.round = recvUint8(wasClosed);    
        game_state.winning_score = recvUint8(wasClosed);    
        return game_state;
    }
    */

    sendLevel(game.level, wasClosed);
    sendUint8(game.current_level, wasClosed);
    sendUint8(game.round, wasClosed);
    sendUint8(game.winning_score, wasClosed);   
} 


/*
armor_t ClientProtocol::readArmor(bool& wasClosed){
    armor_t armor;
    armor.pos = readPosition(wasClosed);
    armor.type = recvUint8(wasClosed);
    armor.is_equipped = recvUint8(wasClosed) ? true : false;
    return armor;
}
*/

void ServerProtocol::sendArmor(armor_t armor, bool &wasClosed) {
    sendPosition(armor.pos, wasClosed);
    sendUint8(armor.type, wasClosed);
    sendUint8(armor.is_equipped, wasClosed);
}

/*
weapon_t ClientProtocol::readWeapon(bool& wasClosed){
    weapon_t weapon;
    weapon.pos = readPosition(wasClosed);
    weapon.type = recvUint8(wasClosed);
    weapon.ammo = recvUint8(wasClosed);
    weapon.is_equipped = recvUint8(wasClosed) ? true : false;
    return weapon;
}

*/
void ServerProtocol::sendWeapon(weapon_t weapon, bool &wasClosed) {
    sendPosition(weapon.pos, wasClosed);
    sendUint8(weapon.type, wasClosed);
    sendUint8(weapon.ammo, wasClosed);
    sendUint8(weapon.is_equipped, wasClosed);
}

/*
position_t ClientProtocol::readPosition(bool& wasClosed){
    position_t position;
    position.x = recvFloat(wasClosed);
    position.y = recvFloat(wasClosed);
    position.w = recvFloat(wasClosed);
    position.h = recvFloat(wasClosed);
    return position;
}
*/

void ServerProtocol::sendPosition(position_t position, bool &wasClosed) {
    sendFloat(position.x, wasClosed);
    sendFloat(position.y, wasClosed);
    sendFloat(position.w, wasClosed);
    sendFloat(position.h, wasClosed);
}


/*
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
    duck.alive = recvUint8(wasClosed) ? true : false;
    duck.score = recvUint8(wasClosed);
    duck.color = recvUint8(wasClosed);
    duck.equipped_weapon = readWeapon(wasClosed);
    duck.equipped_armor = readArmor(wasClosed);
    return duck;
}
*/
void ServerProtocol::sendDuck(duck_t duck, bool &wasClosed) {
    sendPosition(duck.pos, wasClosed);
    sendUint8(duck.id, wasClosed);
    sendUint8(duck.faceLeft, wasClosed);
    sendUint8(duck.isJumping, wasClosed);
    sendUint8(duck.isDucking, wasClosed);
    sendUint8(duck.isFalling, wasClosed);
    sendUint8(duck.isFlaping, wasClosed);
    sendUint8(duck.health, wasClosed);
    sendUint8(duck.isAlive, wasClosed);
    sendUint8(duck.score, wasClosed);
    sendUint8(duck.color, wasClosed);
    sendWeapon(duck.equipped_weapon, wasClosed);
    sendArmor(duck.equipped_armor, wasClosed);
}

/*
void ClientProtocol::readDucks(duck_t ducks[MAX_DUCKS], uint8_t numDucks, bool &wasClosed) {
    for (uint8_t i = 0; i < numDucks; i++) {
        ducks[i] = readDuck(wasClosed);
    }
}
*/

void ServerProtocol::sendDucks(duck_t ducks[MAX_DUCKS], uint8_t num_ducks, bool &wasClosed) {
    sendUint8(num_ducks, wasClosed);
    for (int i = 0; i < num_ducks; i++) {
        sendDuck(ducks[i], wasClosed);
    }
}

/*
void ClientProtocol::readPlatforms(platform_t platforms[MAX_PLATFORMS], uint8_t numPlatforms, bool &wasClosed) {
    for (uint8_t i = 0; i < numPlatforms; i++) {
        platforms[i].platform = readPosition(wasClosed);
    }
}
*/

void ServerProtocol::sendPlatforms(platform_t platforms[MAX_PLATFORMS], uint8_t num_platforms, bool &wasClosed) {
    sendUint8(num_platforms, wasClosed);
    for (int i = 0; i < num_platforms; i++) {
        sendPosition(platforms[i].platform, wasClosed);
    }
}

/*

void ClientProtocol::readSpawnPlaces(spawn_place_t spawn_places[MAX_SPAWN_PLACES], uint8_t numSpawnPlaces, bool &wasClosed) {
    for (uint8_t i = 0; i < numSpawnPlaces; i++) {
        spawn_places[i].pos = readPosition(wasClosed);
        spawn_places[i].is_active = recvUint8(wasClosed);
        spawn_places[i].weapon = readWeapon(wasClosed);
        spawn_places[i].armor = readArmor(wasClosed);
    }
}
*/

void ServerProtocol::sendSpawnPlaces(spawn_place_t spawn_places[MAX_SPAWN_PLACES], uint8_t num_spawn_places, bool &wasClosed) {
    sendUint8(num_spawn_places, wasClosed);
    for (int i = 0; i < num_spawn_places; i++) {
        sendPosition(spawn_places[i].pos, wasClosed);
        sendUint8(spawn_places[i].is_active, wasClosed);
        sendWeapon(spawn_places[i].weapon, wasClosed);
        sendArmor(spawn_places[i].armor, wasClosed);
    }
}

/*
void ClientProtocol::readBoxes(box_t boxes[MAX_BOXES], uint8_t numBoxes, bool &wasClosed) {
    for (uint8_t i = 0; i < numBoxes; i++) {
        boxes[i].pos = readPosition(wasClosed);
        boxes[i].is_explosive = recvUint8(wasClosed);
        boxes[i].is_destroyed = recvUint8(wasClosed);
        boxes[i].weapon = readWeapon(wasClosed);
        boxes[i].armor = readArmor(wasClosed);
    }
}
*/

void ServerProtocol::sendBoxes(box_t boxes[MAX_BOXES], uint8_t num_boxes, bool &wasClosed) {
    sendUint8(num_boxes, wasClosed);
    for (int i = 0; i < num_boxes; i++) {
        sendPosition(boxes[i].pos, wasClosed);
        sendUint8(boxes[i].is_explosive, wasClosed);
        sendUint8(boxes[i].is_destroyed, wasClosed);
        sendWeapon(boxes[i].weapon, wasClosed);
        sendArmor(boxes[i].armor, wasClosed);
    }
}   

/*

void ClientProtocol::readProjectiles(projectile_t projectiles[MAX_PROJECTILES], uint8_t numProjectiles, bool &wasClosed) {
    for (uint8_t i = 0; i < numProjectiles; i++) {
        projectiles[i].pos = readPosition(wasClosed);
        projectiles[i].type = recvUint8(wasClosed);
        projectiles[i].is_active = recvUint8(wasClosed);
    }
}
*/

void ServerProtocol::sendProjectiles(projectile_t projectiles[MAX_PROJECTILES], uint8_t num_projectiles, bool &wasClosed) {
    sendUint8(num_projectiles, wasClosed);
    for (int i = 0; i < num_projectiles; i++) {
        sendPosition(projectiles[i].pos, wasClosed);
        sendUint8(projectiles[i].type, wasClosed);
        sendUint8(projectiles[i].is_active, wasClosed);
    }
}

/*
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
*/


void ServerProtocol::sendLevel(level_t& level, bool &wasClosed) {
    sendUint8(level.num_ducks, wasClosed);
    sendDucks(level.ducks, level.num_ducks, wasClosed);
    sendUint8(level.num_platforms, wasClosed);
    sendPlatforms(level.platforms, level.num_platforms, wasClosed);
    sendUint8(level.num_spawn_places, wasClosed);
    sendSpawnPlaces(level.spawn_places, level.num_spawn_places, wasClosed);
    sendUint8(level.num_boxes, wasClosed);
    sendBoxes(level.boxes, level.num_boxes, wasClosed);
    sendUint8(level.num_projectiles, wasClosed);
    sendProjectiles(level.projectiles, level.num_projectiles, wasClosed);
}