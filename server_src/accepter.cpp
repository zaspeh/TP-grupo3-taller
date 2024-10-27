#include "accepter.h"

#include <iostream>
#include <memory>

#include "monitor.h"
#include "receiver.h"
#include "server.h"

Accepter::Accepter(int port, Server& server, Monitor& monitor):
        socket_servidor(std::to_string(port).c_str()), server(server), monitor(monitor) {}

void Accepter::run() {
    while (_keep_running) {
        try {
            Socket socket_cliente = socket_servidor.accept();
            if (!_keep_running)
                break;
            auto protocol = std::make_shared<ServerProtocol>(std::move(socket_cliente));
            server.addClient(protocol);
            auto client_queue = std::make_shared<Queue<ClientMessage>>(MAX_CLIENTS_PER_QUEUE);
            auto sender = std::make_shared<Sender>(server, monitor, protocol, client_queue);
            sender->start();
            emisores.push_back(sender);
            auto receiver = std::make_shared<Receiver>(server, protocol);
            receiver->start();
            receptores.push_back(receiver);
        } catch (const std::exception& e) {
            std::cerr << "Error accepter: " << e.what() << std::endl;
            break;
        }
    }
}


void Accepter::stop() {
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

Accepter::~Accepter() {
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
