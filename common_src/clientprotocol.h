#include "protocol.h"
#include "game_state.h"

// Esta clase va a tener herencia de Protocol
class ClientProtocol : public Protocol {
private:
    uint8_t client_identifier;

public:
    void sendCommand(uint8_t num, bool &wasClosed);
    GameState readFromServer(bool &wasClosed);

    ClientProtocol(Socket socket, uint8_t client_id);
};
