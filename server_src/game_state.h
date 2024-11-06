#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include "player_state.h"
#include "../common_src/utils.h"
//#include "box.h"
//#include "spawnPoint.h"

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
        /*
typedef struct {
    uint8_t num_ducks;
    duck_t ducks[MAX_DUCKS];
    uint8_t num_platforms;
    platform_t platforms[MAX_PLATFORMS];
    uint8_t num_spawn_places;
    spawn_place_t spawn_places[MAX_SPAWN_PLACES];
    uint8_t num_boxes;
    box_t boxes[MAX_BOXES];
    uint8_t num_projectiles;
    projectile_t projectiles[MAX_PROJECTILES];
} level_t;
*/

        level_t currentLevel = instanceLevel(); // posteriormente deberìa agarrar uno random de arriba.
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
        players.erase(id);
    }

    void updateState(uint8_t id, std::shared_ptr<PlayerState> player) // habria que chequear si agarro una caja, y demas
    {
        state.level.ducks[id] = player->getState();
        
        //std::cout << "Posicion del pato - updatestate: " << state.level.ducks[id].pos.x << " " << state.level.ducks[id].pos.y << std::endl;
    }

    game_state_t doAction(uint8_t id, uint8_t action) {
        std::lock_guard<std::mutex> lock(mtx);
        auto player = players[id];
        
        //if (!player) return;
        Weapon* weapon = nullptr;
        switch(action) {
            case MOVE_LEFT:
                player->move(-5, 0);
                player->setFacingDirection(1);
                break;
            case MOVE_RIGHT:
                std::cout << "Moving right" << std::endl;
                player->move(5, 0);
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
            default:
                std::cout << "Unknown action: " << action << std::endl;
                break;
        }

        updateState(id, player);

        return state;
    }

    game_state_t updatePlayers(float deltaTime) {
        std::lock_guard<std::mutex> lock(mtx);
        
        // Limitar deltaTime para la física
        deltaTime = std::min(deltaTime, 0.033f); // Máximo ~30 FPS
        
        // Actualizar cada jugador
        for (auto& [id, player] : players) {
            player->updatePosition(deltaTime);
            updateState(id, player);
        }
        
        return state;
    }

    game_state_t* firstTime(uint8_t id) {
        std::lock_guard<std::mutex> lock(mtx);  // Garantiza seguridad en un entorno multihilo
        auto it = players.find(id);
        if (it == players.end()) {  // Si el jugador no existe
            std::cout << "Nuevo jugador: " << static_cast<int>(id) << std::endl;
            players[id] = std::make_shared<PlayerState>(100, 100, 32, 32);  // Inicializa el jugador
            state.level.ducks[id] = players[id]->getState();
            state.level.num_ducks++;  // Aumenta el número de "ducks" (jugadores)
            return &state;  // Retorna el estado del juego para el cliente
        }
        return nullptr;  // Si ya está conectado, no realiza nada
    }


/*     game_state_t* isPlayerConnected(uint8_t id) {
        std::lock_guard<std::mutex> lock(mtx);
        if (players.count(id) == 0) {
            std::cout << "Nuevo jugador: " << static_cast<int>(id) << std::endl;
            players[id] = std::make_shared<PlayerState>(100, 100, 32, 32);
            state.level.ducks[id] = players[id]->getState();
            state.level.num_ducks++;
            return &state;
        }
        return nullptr;
    } */

    level_t instanceLevel() {
        std::lock_guard<std::mutex> lock(mtx);

        level_t level = {
            0,                          // num_ducks
            {},                         // ducks array (inicialización vacía)
            0,                          // num_platforms
            {},                         // platforms array (inicialización vacía)
            0,                          // num_spawn_places
            {},                         // spawn_places array (inicialización vacía)
            0,                          // num_boxes
            {},                         // boxes array (inicialización vacía)
            0,                          // num_projectiles
            {}                          // projectiles array (inicialización vacía)
        };

        return level;
    }

};

#endif // GAME_STATE_H