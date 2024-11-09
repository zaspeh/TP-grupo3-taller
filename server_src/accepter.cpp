#include "accepter.h"
#include <iostream>

Accepter::Accepter(int port, Server& server, std::shared_ptr<Monitor> monitor, std::shared_ptr<GameLoop> gameLoop)
    : socket_servidor(std::to_string(port).c_str()), server(server), monitor(monitor), gameLoop(gameLoop), idClient(0) {}

void Accepter::run() {
    while (_keep_running) {
        try {
            std::cout << "Esperando conexiones" << std::endl;
            Socket socket_cliente = socket_servidor.accept();
            std::cout << "Cliente conectado" << std::endl;

            auto protocol = std::make_shared<ServerProtocol>(std::move(socket_cliente));
            auto client = std::make_shared<Client>(server, monitor, gameLoop, protocol, idClient);
            client->start();
            
            server.addClient(idClient++, client);
        } catch (const std::exception& e) {
            std::cerr << "Error accepter: " << e.what() << std::endl;
        }
    }
}

void Accepter::stop() {
    Thread::stop();
    socket_servidor.shutdown(SHUT_RDWR);
    socket_servidor.close();
}

Accepter::~Accepter() {
    stop();
}
