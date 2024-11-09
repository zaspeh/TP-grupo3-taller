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
    //std::vector<std::shared_ptr<level_t>> levels;
    level_t currentLevel;
    mutable std::mutex mtx;

public:
    GameState() {
        players = std::map<uint8_t, std::shared_ptr<PlayerState>>();
        //levels = std::vector<std::shared_ptr<level_t>>(); // almacena todos los niveles del juego

        level_t currentLevel = Level(0).getLevel(); 
        state = {
            currentLevel,
            0,
            0,
            10
        };
    }
    
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

    void removePlayer(uint8_t id) {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Pato eliminado: " << static_cast<int>(id) << "\n";
        // Reducir el contador de num_ducks solo si el jugador existía
        if (players.count(id)) {
            players.erase(id);

            // Reducir el número de patos en el estado del nivel si es mayor que 0
            if (state.level.num_ducks > 0) {
                state.level.num_ducks--;
            }

            // Eliminar el estado del jugador de ducks en el nivel
            state.level.ducks[id].isAlive = false;
        }
    }


    void updateState(uint8_t id, std::shared_ptr<PlayerState> player) 
    {
        state.level.ducks[id] = player->getState();
    }

    std::shared_ptr<PlayerState> connectPlayer(uint8_t id) {
        std::cout << "Nuevo jugador: " << static_cast<int>(id) << std::endl;
<<<<<<< HEAD
        players[id] = std::make_shared<PlayerState>(id, 100, 100, 32, 32);  
=======
        players[id] = std::make_shared<PlayerState>(id, 210, 100, 32, 32);  
>>>>>>> origin/Server_2
        state.level.ducks[id] = players[id]->getState();
        state.level.num_ducks++;  

        std::cout << "Cantidad de jugadores: "<< static_cast<int>(state.level.num_ducks) << std::endl;
        return players[id];
    }

    std::map<uint8_t, std::shared_ptr<PlayerState>> getPlayers() {
        std::lock_guard<std::mutex> lock(mtx);
        return players;
    }

    game_state_t doAction(uint8_t id, uint8_t action) {
        std::cout << "A punto de realizar una acción\n";
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Realizando acción\n";
        auto player = players[id];
        
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
        std::cout << "Accion realizada\n";
        return state;
    }

    game_state_t updatePlayers(float deltaTime) {
        //std::lock_guard<std::mutex> lock(mtx);
        
        // Limitar deltaTime para la física
        deltaTime = std::min(deltaTime, 0.033f); // Máximo ~30 FPS
        
        // Actualizar cada jugador
        for (auto& [id, player] : players) {
            player->updatePosition(deltaTime, state.level.platforms, state.level.num_platforms);
            updateState(id, player); 
        }
        return state;
    }
    

};

#endif // GAME_STATE_H