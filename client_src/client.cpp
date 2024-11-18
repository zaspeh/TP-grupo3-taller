#include "client.h"

std::atomic<uint16_t> Client::next_id(0);

Client::Client(const std::string& server_ip, const std::string& server_port)
        : socket(server_ip.c_str(), server_port.c_str()),
          gameStateQueue(std::make_shared<Queue<game_state_t>>(100)), 
          commandQueue(std::make_shared<Queue<uint8_t>>(100)), _keep_running(true)
    {
        requestId();
        clientprotocol = std::make_shared<ClientProtocol>(std::move(socket), client_id);

        recvThread = std::make_unique<Receiver>(clientprotocol, gameStateQueue);
        recvThread->start();
        
        sendThread = std::make_unique<Sender>(clientprotocol, commandQueue); 
        sendThread->start();


        gameThread = std::make_unique<Game>(gameStateQueue, commandQueue, *this);
        gameThread->start();
    }

void Client::checkIfClose() {
    std::string input;
    std::cout << "Ingrese 'q' para cerrar el juego: ";
    while (std::getline(std::cin, input) && input != "q" && _keep_running) {
        std::cout << "Entrada inválida. Intente nuevamente: ";
    }
    std::cout << "Saliendo de checkIfClose" << std::endl;
    stop();
}

void Client::requestId() {
    unsigned int id_input;
    std::cout << "Ingrese el ID de cliente: ";
    std::cin >> id_input;

    if (id_input > 255) {
        std::cerr << "ID inválido. Debe estar en el rango [0, 255]." << std::endl;
        throw std::invalid_argument("ID fuera de rango");
    }

    client_id = static_cast<uint8_t>(id_input);
    commandQueue->push(NEW_CLIENT); 
}

void Client::run() {
    checkIfClose();
}

void Client::stop(){
    if (!_keep_running) return;
    std::cout << "Client stopped" << std::endl;
    _keep_running.store(false); 
    gameThread->stop();
    sendThread->stop();
    recvThread->stop();
    
    gameThread->join();
    sendThread->join();
    recvThread->join();
}

Client::~Client() {
    try {
        stop();
    } catch (const std::exception& e) {
        std::cerr << "Error al cerrar el juego: " << e.what() << std::endl;
    }
}