#include "Bot.hpp"
#include <iostream>


void Bot::connectToServer(const std::string& host, int port) {
    std::cout << "Bot connecting to server at " << host << ":" << port << std::endl;
}

void Bot::sendMessage(const std::string& message) {
    std::cout << "Bot sending message: " << message << std::endl;
}