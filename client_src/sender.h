#ifndef SENDER_H
#define SENDER_H

#include <string>
#include <iostream>
#include <memory>
#include <cstdint> 
// Own libraries
#include "../common_src/thread.h"
#include "../common_src/queue.h"
#include "client_protocol.h"
#include "../common_src/utils.h"

class Sender : public Thread 
{
private:
    std::shared_ptr<ClientProtocol> protocol;
    std::shared_ptr<Queue<uint8_t>> commandQueue;

    bool commandIsValid(uint8_t command);
public:    
    Sender(std::shared_ptr<ClientProtocol> protocol, std::shared_ptr<Queue<uint8_t>> queue);

    void run() override;
    void stop() override;
};

#endif // SENDER_H
