
#include "server.h"

#include <string>

constexpr const char* SALIR = "q";

Server::Server(int port): gameShouldContinue(true), gameloop(*this, monitor, gameShouldContinue), monitor(*this), accepter(port, *this, monitor, gameloop) {}

std::vector<std::shared_ptr<Sender>>& Server::obtener_emisores() {
    return accepter.obtener_emisores();
}

void Server::handleInput() {
    try {
        std::string input;
        while (_keep_running && gameShouldContinue) {
            if (std::cin.rdbuf()->in_avail()) {  
                std::getline(std::cin, input);
                if (input == SALIR) {
                    stop();
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));  
        }
        if (!gameShouldContinue) {
            stop();  
        }
    } catch (const std::exception& e) {
        stop();
    }
}

void Server::run() {
    try {
        accepter.start();
        gameloop.start();
        handleInput();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        stop();
    }
}

void Server::closeClients() { monitor.cerrar_clientes(); }

void Server::addClient(std::shared_ptr<ServerProtocol> client) { monitor.agregar_cliente(client); }

void Server::removeClient(std::shared_ptr<ServerProtocol> client) {
    monitor.eliminar_cliente(client);
}

std::vector<std::shared_ptr<ServerProtocol>> Server::getClients() {
    return monitor.obtener_clientes();
} 

void Server::stop() {
    try {
        if (!_keep_running) return;
        Thread::stop();
        gameloop.stop();
        accepter.stop();
        accepter.join();
        gameloop.join();
        monitor.cerrar_clientes();
    } catch (const std::exception& e) {
        std::cerr << "Error server: " << e.what() << std::endl;
    }
}

Server::~Server() {
    try {
        stop();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
