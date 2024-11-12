#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include "player_state.h"
#include "level.h"
#include "../common_src/utils.h"

class GameState {
private:
    game_state_t state;
    std::map<uint8_t, std::shared_ptr<PlayerState>> players;
    Level level;
    level_t currentLevel;
    mutable std::mutex mtx;

public:
    // Constructor
    GameState();

    std::shared_ptr<PlayerState> getPlayer(uint8_t id);
    void removePlayer(uint8_t id);
    void updateState(uint8_t id, std::shared_ptr<PlayerState> player);
    std::shared_ptr<PlayerState> connectPlayer(uint8_t id);
    std::map<uint8_t, std::shared_ptr<PlayerState>> getPlayers();
    game_state_t doAction(uint8_t id, uint8_t action);
    game_state_t updatePlayers(float deltaTime);
    weapon_t getWeaponPosition(position_t position);
    Weapon* createWeapon(uint8_t weaponType);
    void checkIfDropWeapon(weapon_t droppedWeapon);
    
};

#endif // GAME_STATE_H
