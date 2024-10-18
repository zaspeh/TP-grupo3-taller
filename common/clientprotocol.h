#include "protocol.h"

// esta clase va a tener herencia de protocolo

class Clientprotocol: public Protocol {
    private:

    public:
        void sendMovement(uint8_t num, bool &wasClosed);
        void sendPlayerName(std::string playerName, bool &wasClosed);
        uint8_t *recvFinalPosition(bool &wasClosed);
        
};