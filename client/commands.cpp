#include "commands.hpp"
#include "client.hpp"
#include "authontification.hpp"


void parseCommand(const std::string& cmd_line, Command& command) {
    size_t start = cmd_line.find_first_not_of(" \t");
    size_t end = cmd_line.find_first_of(" \t", start);

    if (start != std::string::npos) {
        command.command = cmd_line.substr(start, end - start);
        start = cmd_line.find_first_not_of(" \t", end);

        while (start != std::string::npos) {
            end = cmd_line.find_first_of(" \t", start);
            command.params.push_back(cmd_line.substr(start, end - start));
            start = cmd_line.find_first_not_of(" \t", end);
        }
    }
}

void executeCommands(Client& client, const Command& command) {
    if (command.command == "NICK" && command.params.size() == 1) {
        client.NICK(command.params[0]);
    } else if (command.command == "USER" && command.params.size() == 1) {
        client.USER(command.params[0]);
    } else if (command.command == "PASS" && command.params.size() == 1) {
        client.PASS(command.params[0]);
    } else {
        std::cerr << "Unknown command or missing parameters: " << command.command << std::endl;
    }
}


int main() {
    Client client;
    std::string input;

    while (client.isAuthenticated() == false) {
        std::cout << "> ";
        std::getline(std::cin, input);

        Command command;
        parseCommand(input, command);
        executeCommands(client, command);
        client.setAuthenticated();
    }
    std::cout << "Client authenticated with nickname: " << client.getNickname() 
              << ", username: " << client.getUsername() 
              << ", password: " << client.getPassword() << std::endl;

    return 0;
}