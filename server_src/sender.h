#ifndef SENDER_H
#define SENDER_H

#include <memory>
#include <string>
#include <unordered_map>

//#include "../common_src/informacion.h"
#include "../common_src/serverprotocol.h"
#include "../common_src/queue.h"
#include "../common_src/thread.h"
#include "client.h"

class Server;
class Client;

class Sender: public Thread {
private:
    Server& server;
    std::shared_ptr<Client> client;

public:
    explicit Sender(Server &server, std::shared_ptr<Client> client);

    // Ejecuta el hilo sender.
    void run() override;

    // Detiene el sender.
    void stop() override;

    // Destruye el servidor, liberando todos los recursos reservados.
    ~Sender();
};

#endif  // SENDER_H