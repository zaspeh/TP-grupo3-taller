#ifndef CLIENT_H
#define CLIENT_H
#include <string>
#include <memory>
#include <iostream>
#include <cstdint>
//Own libraries
#include "client_protocol.h"
#include "../common_src/queue.h"
#include "sender.h"
#include "receiver.h"
#include "game.h"

class Client {
	private:
        Socket socket;
        uint8_t client_id;
        static uint8_t next_id;
        std::shared_ptr<ClientProtocol> clientprotocol;
        std::shared_ptr<Queue<game_state_t>> gameStateQueue;
        std::shared_ptr<Queue<uint8_t>> commandQueue;
        Sender sendThread;
        Receiver recvThread;
        Game gameThread;


    public:
    	Client(int server_port, const std::string& server_ip); 
        void run();
        void stop();
};

#endif
