#ifndef CLIENT_H
#define CLIENT_H
#include <string>
#include <memory>
//Own libraries
#include "clientprotocol.h"

class Client {
	private:
		uint8_t client_id;
		static uint8_t next_id;
		ClientProtocol clientprotocol;
        std::make_shared<Queue<GameState>> gameStateQueue;
        std::make_shared<Queue<uint8_t>> commandQueue;
        Sender SendThread;
        Receiver RecvThread;
        Game GameThread;


    public:
    	Client(uint16_t server_port, const std::string& server_ip) {} 
        void run();
        void stop();
}

uint8_t Client::next_id = 1;

#endif
