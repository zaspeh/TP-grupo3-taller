#include "protocol.h"

// esta clase va a tener herencia de protocolo

class Serverprotocol: public Protocol {
    private:

    public:
        uint8_t recvMovement(bool &wasClosed);
        std::string recvPlayerName(bool &wasClosed);
        void sendFinalPosition( uint8_t *position ,bool &wasClosed);
        
};