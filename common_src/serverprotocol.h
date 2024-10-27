#ifndef SERVERPROTOCOL_H
#define SERVERPROTOCOL_H

#include "protocol.h"

// esta clase va a tener herencia de protocolo

class ServerProtocol: public Protocol {
    private:

    public:
        ServerProtocol(Socket socket);
        std::vector<uint8_t> recvMovement(bool &wasClosed);
        void sendFinalPosition(std::vector<uint8_t> position ,bool &wasClosed);
        
};

#endif // SERVERPROTOCOL_H