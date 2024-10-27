#include "receiver.h"

#include <iostream>

#include "server.h"

Receiver::Receiver(Server& server, std::shared_ptr<ServerProtocol> protocol):
        server(server), protocol(protocol) {}

void Receiver::run() {
    bool esta_cerrado = false;

    while (_keep_running && !esta_cerrado && server.esta_corriendo()) {
        try {
            std::vector<uint8_t> mensaje = protocol->recvMovement(esta_cerrado);
            std::cout << "Movement: " << static_cast<int>(mensaje[0]) << std::endl;
            std::cout << "Movement: " << static_cast<int>(mensaje[1]) << std::endl;
            if (esta_cerrado) {
                break;
            }

            server.obtener_gameloop().agregar_comando([this, mensaje]() {
                server.obtener_monitor().procesar_mensaje(mensaje);
            });

            
        } catch (const std::exception& e) {
            std::cerr << EXCEPTION << " receiver - " << e.what() << std::endl;
            break;
        }
    }

    server.removeClient(protocol);
}