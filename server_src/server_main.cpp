#include "server.h"
#include <cstdlib>
#include <ctime>

const int CANTIDAD_ARGS = 2;
const int PRIMER_ARGUMENTO = 0;
const int SEGUNDO_ARGUMENTO = 1;
const int ERROR = 1;
const int SUCCESS = 0;

void verificar_argumentos(int argc, const char** argv) {
    if (argc != CANTIDAD_ARGS) {
        std::cout << "Uso: " << argv[PRIMER_ARGUMENTO] << " <puerto>" << std::endl;
        exit(ERROR);
    }
}

int main(int argc, const char** argv) {
    verificar_argumentos(argc, argv);

    try {
        Server server(std::stoi(argv[SEGUNDO_ARGUMENTO]));
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return ERROR;
    }

    return SUCCESS;
}