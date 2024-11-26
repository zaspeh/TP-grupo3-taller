#ifndef ACCEPTER_H
#define ACCEPTER_H

#include <atomic>
#include <memory>
#include <utility>
#include <vector>
#include <arpa/inet.h>

#include "receiver.h"
#include "sender.h"
#include "monitor.h"
#include "../common_src/utils.h"
#include "../common_src/socket.h"
#include "../common_src/thread.h"
#include "../common_src/serverprotocol.h"

class Server;

class Accepter: public Thread {
private:
    Socket socket_servidor;
    Server& server;
    Monitor& monitor;
    GameLoop & gameLoop;
    std::vector<std::shared_ptr<Sender>> emisores;
    std::vector<std::shared_ptr<Receiver>> receptores;
    int clientID;

public:
    Accepter(int port, Server& server, Monitor& monitor, GameLoop& gameLoop);
    std::vector<std::shared_ptr<Sender>>& obtener_emisores() { return emisores; }
    void run() override;
    void stop() override;

    ~Accepter();
};

#endif  // ACCEPTER_H