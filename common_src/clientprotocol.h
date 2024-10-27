#include "protocol.h"

// Esta clase va a tener herencia de Protocol
class ClientProtocol : public Protocol {
private:
    uint8_t client_identifier;

public:
    void sendControlUsed(uint8_t num, bool &wasClosed);
    uint8_t readFromServer(bool &wasClosed);
    std::vector<uint8_t> recvFinalPosition(bool &wasClosed);

    ClientProtocol(Socket socket, uint8_t client_id);
};
