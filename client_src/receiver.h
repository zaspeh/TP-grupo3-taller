#ifndef RECEIVER_H
#define RECEIVER_H

#include <memory>

// Own libraries
#include "common/thread.h"
#include "common/queue.h"
#include "common/game_state.h"
#include "common/client_protocol.h"

class Receiver : public Thread
{
    private:
        std::shared_ptr<ClientProtocol> protocol;
        std::shared_ptr<Queue<GameState>> gameStateQueue;

public:
    Receiver(std::shared_ptr<ClientProtocol> protocol, std::shared_ptr<Queue<GameState>> queue) : protocol(protocol), gameStateQueue(queue) {};
    ~Receiver();
    recvGameState();
};

#endif