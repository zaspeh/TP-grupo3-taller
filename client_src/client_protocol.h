#ifndef CLIENTPROTOCOL_H
#define CLIENTPROTOCOL_H

#include "../common_src/protocol.h"
#include "../common_src/game_state.h"
#include "../common_src/utils.h"
#include <iostream>
#include <vector>
#include <cstdint> 

// Esta clase va a tener herencia de Protocol
class ClientProtocol : public Protocol {
private:
    uint8_t client_identifier;

public:
    ClientProtocol(Socket&& socket, uint8_t client_id);

    void sendCommand(uint8_t num, bool &wasClosed);
    game_state_t readFromServer(bool &wasClosed);
    
    armor_t readArmor(bool &wasClosed);
    weapon_t readWeapon(bool &wasClosed);
    position_t readPosition(bool &wasClosed);
    duck_t readDuck(bool &wasClosed);
    void readDucks(duck_t ducks[MAX_DUCKS], uint8_t numDucks, bool &wasClosed);
    void readPlatforms(platform_t platforms[MAX_PLATFORMS], uint8_t numPlatforms, bool &wasClosed);
    void readSpawnPlaces(spawn_place_t spawn_places[MAX_SPAWN_PLACES], uint8_t numSpawnPlaces, bool &wasClosed);
    void readBoxes(box_t boxes[MAX_BOXES], uint8_t numBoxes, bool &wasClosed);
    void readProjectiles(projectile_t projectiles[MAX_PROJECTILES], uint8_t numProjectiles, bool &wasClosed);
    void readLevel(level_t &level, bool &wasClosed);
};

#endif // CLIENTPROTOCOL_H
