#include "includes.hpp"

void authentificate(Client& client,const std::string& serv_pass, const std::string& commandLine) {
    Command command;
    parseCommand(commandLine, command);
    // {DEBUG}: std::cout << "Authenticating client with command: " << command.command << "---------------" << command.params.size() << std::endl;
    if (command.command == "NICK" && command.params.size() == 1) {
        client.NICK(client.getFd(), command.params[0]);
    } else if ((command.command == "USER" || command.command == "userhost") /*&& command.params.size() == 1*/) {
        client.USER(client.getFd(), command.params[0]);
    } else if (command.command == "PASS" && command.params.size() == 1) {
        client.PASS(client.getFd(), command.params[0], serv_pass);
    }
    if (client.isAuthenticated()) {
        std::cout << "Client authenticated successfully!" << std::endl;
    } 
}

void HandleCommand(int fd, Client& client, const std::string& serv_pass, const std::string& commandLine) {
    Command command;
    client.setFd(fd);
    std::cout << client.Authentification::isAuthenticated() << "  " << commandLine << std::endl;
    if (!client.isAuthenticated()) {
            authentificate(client, serv_pass, commandLine);
        if (!client.isAuthenticated()) {
            std::cerr << "Client not authenticated. ALL Commands are ignored.for user : "<< fd << " except PASS,NICK,USER" << std::endl;
            return;
        }
    }
    // else
    // {
        parseCommand(commandLine, command);
        std::cout << "client +++++++++++++++++++++++++++++++: " << client.getFd() << "  " << client.getNickname() << "  " << client.getUsername() << "  " << client.getPassword() <<std::endl;
        executeCommands(client, command);
    // }
    
}

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



void PRIVMSG(Client& client, const Command& command) {(void)client; // Suppress unused parameter warning
    if (command.params.size() != 2) {
        std::cerr << "PRIVMSG <recipient> <message>." << std::endl;
        return;
    }

    const std::string& recipient = command.params[0];
    const std::string& message = command.params[1];

    // Construct the message to send
    std::string fullMessage = "PRIVMSG to " + recipient + " :" + message + "  Sent successfully!\r\n";
    std::cout << fullMessage << std::endl; // For demonstration, print the message to stdout
    // client.sendMessage(fullMessage);
}

void executeCommands(Client& client, const Command& command) {
    if (command.command == "PRIVMSG") {
        std::string reply = "PONG :server\r\n";
        send(client.getFd(), reply.c_str(), reply.length(), 0);
        // PRIVMSG(client, command);
    } else {
        std::cerr << "Unknown command: " << command.command << std::endl;
    }
}

// int main() {
//     Client client;
//     Command command;
//     client.setFd(3); // Set the file descriptor to stdout for demonstration
//     std::string input;
//     while (true) {
//         std::cout << "> ";
//         std::getline(std::cin, input);

//         if (input == "exit") {
//             break;
//         }

//         parseCommand(input, command);
//         executeCommands(client, command);
//         command.params.clear(); // Clear parameters for the next command
        
//     }

//     return 0;
// }