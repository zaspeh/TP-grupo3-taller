#include "sender.h"

#include <string>
#include <utility>

#include "monitor.h"
#include "server.h"

Sender::Sender(Server& server, Monitor& monitor, std::shared_ptr<ServerProtocol> protocol,
               std::shared_ptr<Queue<ClientMessage>> cola_mensajes):
        server(server), monitor(monitor), protocol(protocol), cola_mensajes(cola_mensajes) {}

void Sender::run() {
    while (_keep_running && server.esta_corriendo()) {
        try {
            bool wasClosed = false;
            GameState mensaje = gameStateQueue->pop();

            if (!server.esta_corriendo())
                break;

            protocol->sendGameState(mensaje.info, wasClosed);

            if (wasClosed) {
                break;
            }

        } catch (const std::exception& e) {
            std::cerr << EXCEPTION << " sender - " << e.what() << std::endl;
        }
    }
}

void Sender::broadcast_message_with_info(const std::vector<uint8_t>& info) {
    cola_mensajes->push(ClientMessage{ClientMessage::MOVEMENT, info});
}

void Sender::stop() {
    Thread::stop();
    ClientMessage msg;
    while (cola_mensajes->try_pop(msg)) {}
    cola_mensajes->close();
}

Sender::~Sender() {
    if (!_keep_running)
        return;

    try {
        Thread::stop();
        ClientMessage msg;
        while (cola_mensajes->try_pop(msg)) {}
        cola_mensajes->close();
    } catch (const std::exception& e) {
        std::cerr << EXCEPTION << e.what() << std::endl;
    }
}