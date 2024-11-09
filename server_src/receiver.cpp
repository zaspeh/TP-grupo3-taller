#include "receiver.h"

#include <iostream>

#include "server.h"

Receiver::Receiver(Server& server, std::shared_ptr<Client> client) :
        server(server), client(client) {}

void Receiver::run() {
    //uint8_t idPlayer = 255;
    while (_keep_running && server.esta_corriendo()) {
        try {
            std::cout << "Esperando comandos" << std::endl;
            std::vector<uint8_t> mensaje = client->recvCommand();
            //idPlayer = mensaje[0];
            
            std::cout << "Received command: " << std::to_string(mensaje[0]) << " " << std::to_string(mensaje[1]) << std::endl;
            client->addCommand(mensaje);
            
        } catch (const std::exception& e) {
            std::cerr << EXCEPTION << " receiver - " << e.what() << std::endl;
            break;
        }
    }
}