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
    Socket &socket;

    void Protocol::checkReceivedStatus(int receivedBytes, bool was_closed,
                                     const std::string& error_message);


protected:


    void Protocol::sendString(const std::string& string, bool &wasClosed);
    void Protocol::sendUint8(uint8_t num, bool &wasClosed);
    void Protocol::sendUint16(uint16_t num, bool &wasClosed);
    std::string Protocol::deserializeString(bool &wasClosed);
    uint8_t Protocol::recvUint8(bool &wasClosed);
    uint16_t Protocol::recvUint16(bool &wasClosed);
    

public:

    Protocol(Socket& socket);

};

#endif