#ifndef SENDER_H
#define SENDER_H

#include <memory>
#include <string>
#include <unordered_map>
#include "../common_src/serverprotocol.h"
#include "../common_src/queue.h"
#include "../common_src/thread.h"

class Server;
class Monitor;

class Sender: public Thread {
private:
    Server& server;
    Monitor& monitor;
    std::shared_ptr<ServerProtocol> protocol;
    std::shared_ptr<Queue<game_state_t>> gameStateQueue;
    std::mutex queue;

public:
    explicit Sender(Server& server, Monitor& monitor, std::shared_ptr<ServerProtocol> protocol,
                    std::shared_ptr<Queue<game_state_t>> gameStateQueue);

    void run() override;
    void broadcast_message_with_info(game_state_t gameState);
    bool isQueueClosed() { return gameStateQueue->isClosed(); }
    void stop() override;    
    ~Sender();
};

#endif  // SENDER_H