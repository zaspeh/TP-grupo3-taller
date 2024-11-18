
#include "server.h"

#include <string>

constexpr const char* SALIR = "q";

Server::Server(int port): gameloop(*this, monitor), monitor(*this), accepter(port, *this, monitor, gameloop) {}

std::vector<std::shared_ptr<Sender>>& Server::obtener_emisores() {
    return accepter.obtener_emisores();
}

void Server::handleInput() {
    try {
        std::string input;
        while (_keep_running) {
            std::getline(std::cin, input);
            if (input == SALIR) {
                stop();
            }
        }
    } catch (const std::exception& e) {
        std::cerr << EXCEPTION << e.what() << std::endl;
        stop();
    }
}


void Server::run() {
    try {
        accepter.start();
        gameloop.start();
        handleInput();

        accepter.join();
        gameloop.join();
    } catch (const std::exception& e) {
        std::cerr << EXCEPTION << " server run - " << e.what() << std::endl;
        stop();
    }
}

void Server::closeClients() { monitor.cerrar_clientes(); }

void Server::addClient(std::shared_ptr<ServerProtocol> client) { monitor.agregar_cliente(client); }

void Server::removeClient(std::shared_ptr<ServerProtocol> client) {
    monitor.eliminar_cliente(client);
}

/* void Server::removeSender(uint8_t idClient) {
    monitor.removeSender(idClient);
} */

std::vector<std::shared_ptr<ServerProtocol>> Server::getClients() {
    return monitor.obtener_clientes();
} 

void Server::stop() {
    Thread::stop();
    monitor.cerrar_clientes();
    accepter.stop();
    gameloop.stop();
}

Server::~Server() {
    if (!_keep_running) {
        return;
    }

    Thread::stop();

    try {
        monitor.cerrar_clientes();
        accepter.stop();
        gameloop.stop();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
