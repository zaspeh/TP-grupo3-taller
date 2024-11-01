#ifndef SENDER_H
#define SENDER_H

#include <string>
#include <memory>
// Own libraries
#include "common/thread.h"
#include "common/queue.h"
#include "common/client_protocol.h"
#include "common/utils.h"
 
class Sender : public Thread 
{
    private:
        std::shared_ptr<ClientProtocol> protocol;
        std::shared_ptr<Queue<uint8_t>> commandQueue;

        bool commandIsValid(uint8_t command);
    public:    
        Sender(std::shared_ptr<ClientProtocol> protocol, std::shared_ptr<Queue<uint8_t>> &queue): protocol(protocol), commandQueue(queue) {};
        void sendCommand();
};


#endif