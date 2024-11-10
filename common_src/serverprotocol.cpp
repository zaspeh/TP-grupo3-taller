#include "serverprotocol.h"

ServerProtocol::ServerProtocol(Socket socket) : Protocol(std::move(socket)) {}

std::vector<uint8_t> ServerProtocol::recvCommand(bool &wasClosed){
    std::vector<uint8_t> buffer(2);
    buffer[0] = recvUint8(wasClosed);
    buffer[1] = recvUint8(wasClosed);
    return buffer;
}

void ServerProtocol::sendGameState(game_state_t& game, bool &wasClosed) {
    sendLevel(game.level, wasClosed);
    sendUint8(game.current_level, wasClosed);
    sendUint8(game.round, wasClosed);
    sendUint8(game.winning_score, wasClosed);   
}

void ServerProtocol::sendArmor(armor_t armor, bool &wasClosed) {
    sendPosition(armor.pos, wasClosed);
    sendUint8(armor.type, wasClosed);
    sendUint8(armor.is_equipped, wasClosed);
}

void ServerProtocol::sendWeapon(weapon_t weapon, bool &wasClosed) {
    sendPosition(weapon.pos, wasClosed);
    sendUint8(weapon.type, wasClosed);
    sendUint8(weapon.ammo, wasClosed);
    sendUint8(weapon.is_equipped, wasClosed);
}

void ServerProtocol::sendPosition(position_t position, bool &wasClosed) {
    sendInt(position.x, wasClosed);
    sendInt(position.y, wasClosed);
}

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

void ServerProtocol::sendDucks(duck_t ducks[MAX_DUCKS], uint8_t num_ducks, bool &wasClosed) {
    for (int i = 0; i < num_ducks; i++) {
        sendDuck(ducks[i], wasClosed);
    }
}

void ServerProtocol::sendPlatforms(platform_t platforms[MAX_PLATFORMS], uint8_t num_platforms, bool &wasClosed) {
    for (int i = 0; i < num_platforms; i++) {
        sendPosition(platforms[i].pos, wasClosed);
        sendUint8(platforms[i].type, wasClosed);
    }
}

void ServerProtocol::sendSpawnPlaces(spawn_place_t spawn_places[MAX_SPAWN_PLACES], uint8_t num_spawn_places, bool &wasClosed) {
    for (int i = 0; i < num_spawn_places; i++) {
        sendPosition(spawn_places[i].pos, wasClosed);
        sendUint8(spawn_places[i].is_active, wasClosed);
        sendWeapon(spawn_places[i].weapon, wasClosed);
        sendArmor(spawn_places[i].armor, wasClosed);
    }
}

void ServerProtocol::sendBoxes(box_t boxes[MAX_BOXES], uint8_t num_boxes, bool &wasClosed) {
    for (int i = 0; i < num_boxes; i++) {
        sendPosition(boxes[i].pos, wasClosed);
        sendUint8(boxes[i].is_explosive, wasClosed);
        sendUint8(boxes[i].is_destroyed, wasClosed);
        sendWeapon(boxes[i].weapon, wasClosed);
        sendArmor(boxes[i].armor, wasClosed);
    }
}   

void ServerProtocol::sendProjectiles(projectile_t projectiles[MAX_PROJECTILES], uint8_t num_projectiles, bool &wasClosed) {
    for (int i = 0; i < num_projectiles; i++) {
        sendPosition(projectiles[i].pos, wasClosed);
        sendUint8(projectiles[i].type, wasClosed);
        sendUint8(projectiles[i].is_active, wasClosed);
    }
}

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