#include "client.h"

void checkIfClose() {
    /* -> con la variable de game y 1 candado
    si se cierra...
    stop();
    */
}

Client(uint16_t server_port, const std::string& server_ip): socket(server_ip.c_str(), std::to_string(server_port).c_str()), client_id(next_id++),
std::make_shared<ClientProtocol>(socket, client_id), gameState(100), commandQueue(100), GameThread(commandQueue, gameStateQueue, *this), RecvThread(clientprotocol, gameStateQueue, *this), SendThread(clientprotocol, commandQueue, *this) {} 


int run() {
    GameThread.start();
    RecvThread.start();
    SendThread.start();

    GameThread.join();
    RecvThread.join();
    SendThread.join();

    checkIfClose();
}


void stop(){
    //monitor.cerrar_clientes();
    GameThread.stop();
    RecvThread.stop();
    SendThread.stop();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << CLIENT_USAGE_COMMAND_ERROR << std::endl;
        return -1;
    }

    Client client(atoi(argv[2]), argv[1]);
    client.run();

    return 0;
}