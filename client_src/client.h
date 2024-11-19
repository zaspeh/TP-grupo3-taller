#ifndef CLIENT_H
#define CLIENT_H
#include <string>
#include <memory>
#include <iostream>
#include <cstdint>
#include <atomic>
//Own libraries
#include "client_protocol.h"
#include "../common_src/queue.h"
#include "sender.h"
#include "receiver.h"
#include "game.h"
#include "../common_src/utils.h"

class Game;

class Client {
	private:
        Socket socket;
        uint8_t client_id;
        static std::atomic<uint16_t> next_id;
        std::shared_ptr<ClientProtocol> clientprotocol;
        std::shared_ptr<Queue<game_state_t>> gameStateQueue;
        std::shared_ptr<Queue<uint8_t>> commandQueue;
        std::unique_ptr<Sender> sendThread;
        std::unique_ptr<Receiver> recvThread;
        std::unique_ptr<Game> gameThread;
        std::atomic<bool> _keep_running;
        void checkIfClose();

    public:
        void requestId();
    	Client(const std::string& server_ip, const std::string& server_port); 
        Client(const Client&) = default; 
        ~Client();
        void run();
        void stop();
};

#endif
