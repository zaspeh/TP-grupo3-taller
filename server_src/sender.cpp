#include "sender.h"

#include <string>
#include <utility>

#include "monitor.h"
#include "server.h"

Sender::Sender(Server& server, Monitor& monitor, std::shared_ptr<ServerProtocol> protocol,
               std::shared_ptr<Queue<game_state_t>> gameStateQueue):
        server(server), monitor(monitor), protocol(protocol), gameStateQueue(gameStateQueue) {}

void Sender::run() {
    while (_keep_running && server.esta_corriendo()) {
        try {
            bool wasClosed = false;
            game_state_t mensaje = gameStateQueue->pop();

            if (protocol == nullptr) {
                break;
            }

            if (!server.esta_corriendo())
                break;
            std::cout << "Cantidad de jugadores: "<< static_cast<int>(mensaje.level.num_ducks) << std::endl;
            //std::cout << "enviando posicion: "<< mensaje.level.ducks[0].pos.x << " "<< mensaje.level.ducks[0].pos.y << std::endl;
            protocol->sendGameState(mensaje, wasClosed);

            if (wasClosed) {
                break;
            }

        } catch (const std::exception& e) {
            std::cerr << EXCEPTION << " sender - " << e.what() << std::endl;
        }
    }
}

void Sender::broadcast_message_with_info(game_state_t gameState) {
    gameStateQueue->push(gameState);
}

void Sender::stop() {
    Thread::stop();
    game_state_t msg;
    while (gameStateQueue->try_pop(msg)) {}
    gameStateQueue->close();
}

Sender::~Sender() {
    if (!_keep_running)
        return;

    try {
        Thread::stop();
        game_state_t msg;
        while (gameStateQueue->try_pop(msg)) {}
        gameStateQueue->close();
    } catch (const std::exception& e) {
        std::cerr << EXCEPTION << e.what() << std::endl;
    }
}