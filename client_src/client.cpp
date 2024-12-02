#include "client.h"
#include <sys/select.h>
#include <unistd.h>

std::atomic<uint16_t> Client::next_id(0);

Client::Client(const std::string& server_ip, const std::string& server_port)
        : socket(server_ip.c_str(), server_port.c_str()),
          gameStateQueue(std::make_shared<Queue<game_state_t>>(100)), 
          commandQueue(std::make_shared<Queue<uint8_t>>(100)), _keep_running(true), clientContinue(true)
    {
        commandQueue->push(NEW_CLIENT);
        clientprotocol = std::make_shared<ClientProtocol>(std::move(socket), client_id);

        recvThread = std::make_unique<Receiver>(clientprotocol, gameStateQueue);
        recvThread->start();
        
        sendThread = std::make_unique<Sender>(clientprotocol, commandQueue); 
        sendThread->start();

        gameThread = std::make_unique<Game>(gameStateQueue, commandQueue, *this);
        gameThread->start();
    }

/* void Client::checkIfClose() {
    while (_keep_running) {
        std::string input;
        std::getline(std::cin, input);  // Directa lectura sin verificación del búfer

        if (input == "q") {
            break;

        }
        std::cout << "Entrada inválida. Intente nuevamente: ";
    }
    stop();
} */

bool inputAvailable() {
    fd_set set;
    struct timeval timeout;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000; 
    return select(STDIN_FILENO + 1, &set, nullptr, nullptr, &timeout) > 0;
}

void Client::checkIfClose() {
    try {
        std::string input;
        while (_keep_running && clientContinue) {
            if (inputAvailable()) {
                std::getline(std::cin, input);
                /* if (input == "q") {
                    stop();
                } */
            }
        }
        if (!clientContinue) {
            stop();  
        }
    } catch (const std::exception& e) {
        stop();
    }
}

void Client::endGame() {
    clientContinue = false;
}

void Client::run() {
    checkIfClose();
}

void Client::stop(){
    try {
        if (!_keep_running) return;
        _keep_running = false;
        sendThread->stop();
        gameThread->stop();
        recvThread->stop();

        gameThread->join();
        sendThread->join();
        recvThread->join();
    } catch (const std::exception& e) {
        std::cerr << "Error Client stop: " << e.what() << std::endl;
    }
}

Client::~Client() {
    try {
        stop();
    } catch (const std::exception& e) {
        std::cerr << "Error al cerrar el juego: " << e.what() << std::endl;
    }
}