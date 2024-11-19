#ifndef RECEIVER_H
#define RECEIVER_H

#include <memory>

// Own libraries
#include "../common_src/thread.h"
#include "../common_src/queue.h"
#include "../common_src/utils.h"
#include "../common_src/game_state.h"
#include "client_protocol.h"

class Receiver : public Thread
{
    private:
        std::shared_ptr<ClientProtocol> protocol;
        std::shared_ptr<Queue<game_state_t>> gameStateQueue;

public:
    Receiver(std::shared_ptr<ClientProtocol> protocol, std::shared_ptr<Queue<game_state_t>> queue);
    
    void run() override;
    void stop() override;
    ~Receiver();
};

#endif