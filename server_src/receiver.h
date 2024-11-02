#ifndef RECEIVER_H
#define RECEIVER_H

#include <memory>

#include "../common_src/serverprotocol.h"
#include "../common_src/thread.h"
#include "gameloop.h"

class Server;

class Receiver: public Thread {
private:
    Server& server;
    std::shared_ptr<ServerProtocol> protocol;
    GameLoop& gameLoop;

public:
    explicit Receiver(Server& server, std::shared_ptr<ServerProtocol> protocol, GameLoop& gameLoop);

    // Ejecuta el hilo receiver.
    void run() override;
};

#endif  // RECEIVER_H