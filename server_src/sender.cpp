#include "sender.h"

#include <string>
#include <utility>

#include "monitor.h"
#include "server.h"

Sender::Sender(Server& server, std::shared_ptr<Client> client) : server(server), client(client) {}

void Sender::run() {
    while (_keep_running && server.esta_corriendo()) {
        game_state_t mensaje;
        try {
            try {
                mensaje = client->getGameState();
            } catch (const std::exception& e) {
                std::cerr << "Error: fallo al recibir mensaje - " << e.what() << std::endl;
                break;
            }

            std::cout << "Cantidad de jugadores: " << static_cast<int>(mensaje.level.num_ducks) << std::endl;

            client->sendGameState(mensaje);
        } catch (const std::exception& e) {
            std::cerr << EXCEPTION << "sender - " << e.what() << std::endl;
        }
    }
}

void Sender::stop() {
    Thread::stop();
}

Sender::~Sender() {
    if (!_keep_running)
        return;

    Thread::stop();
}