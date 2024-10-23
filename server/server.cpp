
#include "server.h"

#include <string>

constexpr const char* SALIR = "q";

Server::Server(int port): aceptador(port, *this, monitor), gameloop(*this), monitor(*this) {}

std::vector<std::shared_ptr<Sender>>& Server::obtener_emisores() {
    return aceptador.obtener_emisores();
}

void Server::manejar_entrada() {
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
        aceptador.start();
        gameloop.start();
        manejar_entrada();

        aceptador.join();
        gameloop.join();
    } catch (const std::exception& e) {
        std::cerr << EXCEPTION << e.what() << std::endl;
        stop();
    }
}

void Server::cerrar_clientes() { monitor.cerrar_clientes(); }

void Server::agregar_cliente(std::shared_ptr<Protocolo> client) { monitor.agregar_cliente(client); }

void Server::eliminar_cliente(std::shared_ptr<Protocolo> client) {
    monitor.eliminar_cliente(client);
}

std::vector<std::shared_ptr<Protocolo>> Server::obtener_clientes() {
    return monitor.obtener_clientes();
}

std::unordered_map<CajaID, Caja>& Server::obtener_cajas() { return cajas; }

void Server::stop() {
    Thread::stop();
    monitor.cerrar_clientes();
    aceptador.stop();
    gameloop.stop();
}

Server::~Server() {
    if (!_keep_running) {
        return;
    }

    Thread::stop();

    try {
        monitor.cerrar_clientes();
        aceptador.stop();
        gameloop.stop();
    } catch (const std::exception& e) {
        std::cerr << EXCEPTION << e.what() << std::endl;
    }
}
