#ifndef ACCEPTER_H
#define ACCEPTER_H

#include <atomic>
#include <memory>
#include <utility>
#include <vector>

#include <arpa/inet.h>

#include "client.h"
#include "gameloop.h"
#include "../common_src/utils.h"
#include "../common_src/socket.h"
#include "../common_src/thread.h"
#include "../common_src/serverprotocol.h"

class Accepter: public Thread {
    private:
        Socket socket_servidor;
        Server& server;
        std::shared_ptr<Monitor> monitor;
        std::shared_ptr<GameLoop> gameLoop;
        uint8_t idClient;

    public:
        explicit Accepter(int port, Server& server, std::shared_ptr<Monitor> monitor, std::shared_ptr<GameLoop> gameLoop);

        // Ejecuta el hilo aceptador de conexiones.
        void run() override;

        // Detiene el hilo aceptador de conexiones.
        void stop() override;

        // Destruye el Accepter, liberanndo todos los recursos reservados.
        ~Accepter();
};

#endif  // ACCEPTER_H