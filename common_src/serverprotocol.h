#ifndef SERVERPROTOCOL_H
#define SERVERPROTOCOL_H

#include "protocol.h"
#include "../server_src/game_state.h"


// esta clase va a tener herencia de protocolo

class ServerProtocol: public Protocol {
    private:
        void sendArmor(armor_t armor, bool &wasClosed);
        void sendWeapon(weapon_t weapon, bool &wasClosed);
        void sendPosition(position_t position, bool &wasClosed);
        void sendDuck(duck_t duck, bool &wasClosed);
        void sendDucks(duck_t ducks[MAX_DUCKS], uint8_t num_ducks, bool &wasClosed);
        void sendPlatforms(platform_t platforms[MAX_PLATFORMS], uint8_t num_platforms, bool &wasClosed);
        void sendSpawnPlaces(spawn_place_t spawn_places[MAX_SPAWN_PLACES], uint8_t num_spawn_places, bool &wasClosed);
        void sendBoxes(box_t boxes[MAX_BOXES], uint8_t num_boxes, bool &wasClosed);
        void sendProjectiles(projectile_t projectiles[MAX_PROJECTILES], uint8_t num_projectiles, bool &wasClosed);
        void sendDroppedWeapons(weapon_t droppedWeapons[MAX_ITEMS], uint8_t numDroppedWeapons, bool wasClosed);
        void sendDroppedArmors(armor_t droppedArmors[MAX_ITEMS], uint8_t numDroppedArmors, bool wasClosed);
        void sendLevel(level_t& level, bool &wasClosed);
        void sendPositions(position_t explosions[MAX_ITEMS], uint8_t numExplosions, bool wasClosed);

    public:
        ServerProtocol(Socket socket);
        std::vector<uint8_t> recvCommand(bool &wasClosed);
        void sendGameState(game_state_t& game,bool &wasClosed);
        
};

#endif // SERVERPROTOCOL_H