#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

#include <arpa/inet.h>

// Own libraries
#include "socket.h"
#include "utils.h"

class Protocol {
private:
    Socket socket;

    void checkReceivedStatus(int receivedBytes, bool was_closed,
                                     const std::string& error_message);


protected:


    void sendString(const std::string& string, bool &wasClosed);
    void sendUint8(uint8_t num, bool &wasClosed);
    void sendUint16(uint16_t num, bool &wasClosed);
    std::string deserializeString(bool &wasClosed);
    uint8_t recvUint8(bool &wasClosed);
    uint16_t recvUint16(bool &wasClosed);
    float recvFloat(bool &wasClosed);
    float sendFloat(float num, bool &wasClosed);

public:

    Protocol(Socket socket);

    void closeSocket();

};

#endif