#include "client/includes.hpp"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: ./server <port> <password>" << std::endl;
        return 1;
    }

    try {
        Server irc_server(std::atoi(argv[1]), argv[2]);
        irc_server.Init();
        irc_server.Run();
    } 
    catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}