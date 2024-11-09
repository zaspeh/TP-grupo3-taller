#include "server.h"
#include "accepter.h"
#include "monitor.h"

#include <string>

constexpr const char* SALIR = "q";

Server::Server(int port) {
    monitor = std::make_shared<Monitor>(*this);
    gameloop = std::make_shared<GameLoop>(*this, monitor);
    accepter = std::make_shared<Accepter>(port, *this, monitor, gameloop);
}

std::map<uint8_t, std::shared_ptr<Client>>& Server::getClients() {
    return clients;
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
        accepter->start();
        gameloop->start();
        handleInput();

        accepter->join();
        gameloop->join();
    } catch (const std::exception& e) {
        std::cerr << EXCEPTION << " server run - " << e.what() << std::endl;
        stop();
    }
}

void Server::closeClients() { monitor->closeClients(clients); }

void Server::addClient(uint8_t idClient, std::shared_ptr<Client> client) { 
    monitor->addToMap(clients, idClient, client); 
}


void Server::removeClient(uint8_t idClient) {
    monitor->removeFromMap(clients, idClient);
}

void Server::stop() {
    Thread::stop();
    closeClients();
    accepter->stop();
    gameloop->stop();
}

Server::~Server() {
    if (!_keep_running) {
        return;
    }

    Thread::stop();

    try {
        closeClients();
        accepter->stop();
        gameloop->stop();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
