#ifndef SERVERPROTOCOL_H
#define SERVERPROTOCOL_H

#include "protocol.h"

// esta clase va a tener herencia de protocolo

class ServerProtocol: public Protocol {
    private:

    public:
        ServerProtocol(Socket socket);
        std::vector<uint8_t> recvCommand(bool &wasClosed);
        void sendGameState(GameState game,bool &wasClosed);
        
};

#endif // SERVERPROTOCOL_H