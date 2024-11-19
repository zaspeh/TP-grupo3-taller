#ifndef SERVER_H
#define SERVER_H

#include <atomic>
#include <memory>
#include <unordered_map>
#include <vector>
// Own libraries
#include "../common_src/socket.h"
#include "../common_src/serverprotocol.h"
#include "../common_src/thread.h"

#include "accepter.h"
#include "gameloop.h"
#include "monitor.h"
#include "receiver.h"
#include "sender.h"
#include "game_state.h"

class Server: public Thread {
private:
    GameLoop gameloop;
    Monitor monitor;
    Accepter accepter;
    void closeClients();
    void handleInput();
    std::vector<std::shared_ptr<ServerProtocol>> getClients();

public:
    explicit Server(int port);
    void run() override;
    void stop() override;
    void addClient(std::shared_ptr<ServerProtocol> client);
    void removeClient(std::shared_ptr<ServerProtocol> client);
    std::vector<std::shared_ptr<Sender>>& obtener_emisores();
    ~Server();
};

#endif  // SERVER_H