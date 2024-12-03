
#include "server.h"
#include <sys/select.h>
#include <unistd.h>

#include <string>

constexpr const char* SALIR = "q";

Server::Server(int port): gameShouldContinue(true), gameloop(*this, monitor, gameShouldContinue), monitor(*this), accepter(port, *this, monitor, gameloop) {}

std::vector<std::shared_ptr<Sender>>& Server::obtener_emisores() {
    return accepter.obtener_emisores();
}

bool inputAvailable() {
    fd_set set;
    struct timeval timeout;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000; 
    return select(STDIN_FILENO + 1, &set, nullptr, nullptr, &timeout) > 0;
}

void Server::handleInput() {
    try {
        std::string input;
        while (_keep_running && gameShouldContinue) {
            if (inputAvailable()) {
                std::getline(std::cin, input);
                if (input == SALIR) {
                    stop();
                }
            }
        }
        if (!gameShouldContinue) {
            //std::cout << "Llamando a stop\n";
            stop();  
        }
    } catch (const std::exception& e) {
        stop();
    }
}

void Server::closeServer() {
    gameShouldContinue = false;
}

void Server::run() {
    try {
        accepter.start();
        gameloop.start();
        handleInput();

    } catch (const std::exception& e) {
        //std::cerr << "Error: " << e.what() << std::endl;
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
