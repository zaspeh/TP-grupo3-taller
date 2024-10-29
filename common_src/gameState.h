#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include "playerState.h"
#include "level.h"
#include "box.h"
#include "spawnPoint.h"

class gameState {
private:
    std::map<uint8_t, std::shared_ptr<PlayerState>> players;  
    std::shared_ptr<level> level;
    mutable std::mutex mtx;

public:
    gameState(std::map<uint8_t, std::shared_ptr<PlayerState>> p, std::shared_ptr<level> l) 
        : players(p), level(l) {}

    void changeLevel(std::shared_ptr<level> l) {
        std::lock_guard<std::mutex> lock(mtx);
        level = l;
    }

    std::shared_ptr<PlayerState> getPlayer(uint8_t id) {
        std::lock_guard<std::mutex> lock(mtx);
        return players.count(id) ? players[id] : nullptr;
    }

    std::shared_ptr<level> getLevel() {
        std::lock_guard<std::mutex> lock(mtx);
        return level;
    }
    
    void addPlayer(uint8_t id, std::shared_ptr<PlayerState> player) {
        std::lock_guard<std::mutex> lock(mtx);
        players[id] = player;
    }

    void removePlayer(uint8_t id) {
        std::lock_guard<std::mutex> lock(mtx);
        players.erase(id);
    }

    void doAction(uint8_t id, uint8_t action) {
        std::lock_guard<std::mutex> lock(mtx);
        auto player = players[id];
        
        if (!player) return;

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
                Weapon weapon = level->findWeapon(player->getPosition());
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
                player->setCrouched(!player->isCrouched());
                break;
            default:
                std::cout << "Unknown action: " << action << std::endl;
                break;
        }
    }

    void updatePlayers(float deltaTime) {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto& [id, player] : players) {
            player->updatePosition(deltaTime);
        }
    }
};
