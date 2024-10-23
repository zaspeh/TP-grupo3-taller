#include "listener.h"

#include <iostream>
#include <memory>

#include "monitor.h"
#include "receiver.h"
#include "server.h"

Listener::Listener(int port, Server& server, Monitor& monitor):
        socket_servidor(std::to_string(port).c_str()), server(server), monitor(monitor) {}

void Listener::run() {
    while (_keep_running) {
        try {
            Socket socket_cliente = socket_servidor.accept();
            if (!_keep_running)
                break;
            auto cliente_ptr = std::make_shared<Protocolo>(std::move(socket_cliente));
            server.agregar_cliente(cliente_ptr);
            auto client_queue = std::make_shared<Queue<ClientMessage>>(100);
            auto sender = std::make_shared<Sender>(server, monitor, cliente_ptr, client_queue);
            sender->start();
            emisores.push_back(sender);
            auto receiver = std::make_shared<Receiver>(server, cliente_ptr);
            receiver->start();
            receptores.push_back(receiver);
        } catch (const std::exception& e) {
            std::cerr << EXCEPTION << e.what() << std::endl;
            break;
        }
    }
}


void Listener::stop() {
    Thread::stop();

    socket_servidor.shutdown(SHUT_RDWR);
    socket_servidor.close();

    for (auto& sender: emisores) {
        sender->stop();
        sender->join();
    }
    for (auto& receiver: receptores) {
        receiver->stop();
        receiver->join();
    }
    emisores.clear();
    receptores.clear();
}

Listener::~Listener() {
    if (!_keep_running)
        return;

    try {
        Thread::stop();

        socket_servidor.shutdown(SHUT_RDWR);
        socket_servidor.close();

        for (auto& sender: emisores) {
            sender->stop();
            sender->join();
        }
        for (auto& receiver: receptores) {
            receiver->stop();
            receiver->join();
        }
        emisores.clear();
        receptores.clear();
    } catch (const std::exception& e) {
        std::cerr << EXCEPTION << e.what() << std::endl;
    }
}