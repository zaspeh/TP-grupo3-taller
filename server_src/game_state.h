#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include "player_state.h"
#include "level.h"
#include "../common_src/utils.h"
//#include "box.h"
//#include "spawnPoint.h"

class GameState {
private:
    game_state_t state;
    std::map<uint8_t, std::shared_ptr<PlayerState>> players;  
    std::vector<std::shared_ptr<level_t>> levels;
    Level level;
    mutable std::mutex mtx;

public:
    GameState(Level l) 
        : level(l) {}

    GameState() = default;

    /* void changeLevel(std::shared_ptr<level_t> l) {
        std::lock_guard<std::mutex> lock(mtx);
        level = l;
    } */

    std::shared_ptr<PlayerState> getPlayer(uint8_t id) {
        std::lock_guard<std::mutex> lock(mtx);
        return players.count(id) ? players[id] : nullptr;
    }

    /* std::shared_ptr<level> getLevel() {
        std::lock_guard<std::mutex> lock(mtx);
        return level;
    } */
    
    level_t& getLevelState() {
        return level.getState();
    }

    void addPlayer(uint8_t id, std::shared_ptr<PlayerState> player) {
        std::lock_guard<std::mutex> lock(mtx);
        players[id] = player;
    }

    void removePlayer(uint8_t id) {
        std::lock_guard<std::mutex> lock(mtx);
        players.erase(id);
    }

    game_state_t doAction(uint8_t id, uint8_t action) {
        std::lock_guard<std::mutex> lock(mtx);
        auto player = players[id];
        
        //if (!player) return;
        Weapon* weapon = nullptr;
        switch(action) {
            case MOVE_LEFT:
                player->move(-1, 0);
                player->setFacingDirection(-1);
                break;
            case MOVE_RIGHT:
                player->move(1, 0);
                player->setFacingDirection(1);
                break;
            case JUMP:
                player->jump();
                break;
            case TAKE_WEAPON:
                //weapon = level->findWeapon(player->getPosition());
                if (weapon)
                    player->pickWeapon(weapon);
                break;
            case SHOOT:
                player->shoot();
                break;
            case LOOK_UP:
                // Implementar lógica para mirar hacia arriba
                break;
            case FLOOR:
                // Implementar lógica para agacharse
                //player->setCrouched(!player->isCrouched());
                break;
            default:
                std::cout << "Unknown action: " << action << std::endl;
                break;
        }

        return state;
    }

    void updatePlayers(float deltaTime) {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto& [id, player] : players) {
            player->updatePosition(deltaTime);
        }
    }

    bool isPlayerConnected(uint8_t id) {
        std::lock_guard<std::mutex> lock(mtx);
        if (players.count(id) == 0) {
            players[id] = std::make_shared<PlayerState>();
            return false;
        }
        return true;
    }
};

#endif // GAME_STATE_H