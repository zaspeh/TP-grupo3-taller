#include "receiver.h"

void Receive::recvGameState(){
    bool wasClosed = false;
    while (!wasClosed && running.load(std::memory_order_acquire)) {
        try {
            GameState state = protocol.readFromServer( wasClosed);
            if (wasClosed)
                break;

            if (state) // tal vez alguna otra validacion 
                gameStateQueue.push(state);
        } catch (const std::exception& e) {
            log_error(e);
            break;
        }
    }
    monitor.remove_client(client_socket);
}


void Receive::Receive(ClientProtocol protocol) : protocol(protocol), recvGameState() {}