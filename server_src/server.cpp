
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
            if (std::cin.rdbuf()->in_avail()) {  // verifica si hay input disponible
                std::getline(std::cin, input);
                if (input == SALIR) {
                    stop();
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));  // pequeña pausa para no consumir CPU
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        stop();
    }
    std::cout << "handle input\n";
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
        std::cout << "Cerrando server\n";
        gameloop.stop();
        accepter.stop();
        accepter.join();
        std::cout << "Server cerrado\n";
        gameloop.join();
        monitor.cerrar_clientes();
        std::cout << "Gameloop acabado\n";
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
