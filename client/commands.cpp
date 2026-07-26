#include "includes.hpp"

void authentificate(Client& client,std::map<int, Client>& clientBuffers, const std::string& serv_pass, const std::string& commandLine) {
    Command command;
    parseCommand(commandLine, command);

    // {DEBUG}: std::cout << "Authenticating client with command: " << command.command << "---------------" << command.params.size() << std::endl;
    if (command.command == "NICK" && command.params.size() == 1) {
         client.NICK(client.getFd(),clientBuffers, command.params[0]); 
    } else if ((command.command == "USER") && command.params.size() == 4) {
        client.USER(client.getFd(),clientBuffers, command.params[0]);
    }
    else if ((command.command == "PASS") && client.getPassword() == serv_pass) {
        std::string reply = "462 ERR_ALREADYREGISTERED.\r\n";
        send(client.getFd(), reply.c_str(), reply.length(), 0);
    }
     else if ((command.command == "PASS") && command.params.size() == 1) {
        client.PASS(client.getFd(), command.params[0], serv_pass);
    }
    // if (client.isAuthenticated()) {
    //     std::string reply = "WELCOME " + client.getUsername() + " :You are now authenticated enjoy!\r\n";
    //     std::cout << "Client " << client.getFd() << " = "<< client.getUsername() << " authenticated successfully!" << std::endl;
    //     send(client.getFd(), reply.c_str(), reply.length(), 0);
    // } 
	if (client.isAuthenticated()) {
		std::string nick = client.getNickname();
		std::string user = client.getUsername();
		std::string host = "localhost"; // or resolved client IP if you have it

		std::string reply;
		reply += ":irc.Brika.net 001 " + nick + " :Welcome to the Internet Relay Network " 
				+ nick + "!" + user + "@" + host + "\r\n";
		reply += ":irc.Brika.net 002 " + nick + " :Your host is irc.Brika.net, running version 1.0\r\n";
		reply += ":irc.Brika.net 003 " + nick + " :This server was created today\r\n";
		reply += ":irc.Brika.net 004 " + nick + " irc.Brika.net 1.0 o o\r\n";

		std::cout << "Client " << client.getFd() << " = " << client.getUsername() 
				<< " authenticated successfully!" << std::endl;
		send(client.getFd(), reply.c_str(), reply.length(), 0);
	}
}
void executeCommands(Client& client, std::map<int, Client>& clientBuffers, const Command& command, ChannelRegistry& channels);
void HandleCommand(int fd, std::map<int, Client>& clientBuffers, const std::string& serv_pass, const std::string& commandLine, ChannelRegistry& channels) {
    Command command;
    Client& client = clientBuffers[fd];
    client.setFd(fd);
    std::cout << client.Authentification::isAuthenticated() << "  " << commandLine << std::endl;
    if (!client.isAuthenticated()) {
            authentificate(client,clientBuffers, serv_pass, commandLine);
        if (!client.isAuthenticated()) {
            std::cerr << "Client not authenticated. ALL Commands are ignored.for user : "<< fd << " except PASS,NICK,USER" << std::endl;
            return;
        }
    }
    else
    {
        parseCommand(commandLine, command);
        // std::cout << "client +++++++++++++++++++++++++++++++: " << client.getFd() << "  " << client.getNickname() << "  " << client.getUsername() << "  " << client.getPassword() <<std::endl;
        executeCommands(client,clientBuffers, command, channels);
    }
    
}

void parseCommand(const std::string& cmd_line, Command& command) {
    size_t start = cmd_line.find_first_not_of(" \t");
    size_t end = cmd_line.find_first_of(" \t", start);

    if (start != std::string::npos) {
        command.command = cmd_line.substr(start, end - start);
        start = cmd_line.find_first_not_of(" \t", end);

        while (start != std::string::npos) {
            if (cmd_line[start] == ':') {
                end = cmd_line.length();
                start++;
            }
            else {
                end = cmd_line.find_first_of(" \t", start);
            }
            command.params.push_back(cmd_line.substr(start, end - start));
            start = cmd_line.find_first_not_of(" \t", end);
        }
    }
}

void PRIVMSG(int fd, const std::map<int, Client>& clientBuffers, Client& client, const Command& command, ChannelRegistry& channels) {
    if (command.params.size() != 2) {
        std::string reply = "PRIVMSG <recipient> <message> . \r\n";
        send(fd,reply.c_str(),reply.length(),0);
        std::cerr << "PRIVMSG <recipient> <message>." << std::endl;
        return;
    }

    const std::string& recipient = command.params[0];
    const std::string& message = command.params[1];

    if (recipient.empty() || message.empty()) {
        std::string reply = "PRIVMSG <recipient> <message> . \r\n";
        send(fd,reply.c_str(),reply.length(),0);
        std::cerr << "PRIVMSG <recipient> <message>." << std::endl;
        return;
    }
    if (recipient[0] == '#') {
        if (!channels.channelExists(recipient)) {
            std::string reply = "Channel " + recipient + " does not exist.\r\n";
            send(fd, reply.c_str(), reply.length(), 0);
            std::cerr << "Channel " << recipient << " does not exist." << std::endl;
            return;
        }
        else {
            Channel* channel = channels.findChannel(recipient);
            if (channel) {
                // Check if the client is part of the channel
                // Assuming Channel class has a method to check membership
                if (!channel->isMember(client.getFd())) {
                    std::string reply = "You are not a member of channel " + recipient + ".\r\n";
                    send(fd, reply.c_str(), reply.length(), 0);
                    std::cerr << "Client " << client.getFd() << " is not a member of channel " << recipient << "." << std::endl;
                    return;
                }
                // :salah!salah@localhost PRIVMSG #general :Hello
                std::string fullMessage = ":" + client.getUsername()+"!"+client.getNickname() + "@localhost" + " PRIVMSG " + recipient + " :" + message + "\r\n";
                channel->broadCast(fullMessage, client.getFd());
                std::cout << "Message sent to channel " << recipient << ": " << message << std::endl;
                return;
            }
        }
        std::string reply = "PRIVMSG to channels is not implemented yet.\r\n";
        send(fd, reply.c_str(), reply.length(), 0);
        std::cerr << "PRIVMSG to channels is not implemented yet." << std::endl;
        return;
    }
    // Find the recipient in the clientBuffers
    for (std::map<int, Client>::const_iterator pair = clientBuffers.begin(); pair != clientBuffers.end(); ++pair) {
        if(pair->second.getNickname() == recipient || pair->second.getUsername() == recipient) {
            // Recipient found, send the message
            std::string fullMessage = "PRIVMSG from " + client.getNickname() + " : " + message + "\r\n";
            send(pair->second.getFd(), fullMessage.c_str(), fullMessage.length(), 0);
            std::cout << "Message sent to " << recipient << ": " << message << std::endl;
            return;
        }
    }

    // If recipient not found
    std::string reply = "Recipient " + recipient + " not found.\r\n";
    send(fd, reply.c_str(), reply.length(), 0);
    std::cerr << "Recipient " << recipient << " not found." << std::endl;
}


void executeCommands(Client& client, std::map<int, Client>& clientBuffers, const Command& command, ChannelRegistry& channels) {
	std::vector<std::string> params = command.params;
    if(command.command == "PASS")
    {
        std::string reply =  "U're already authentified ! \r\n";
        send(client.getFd(),reply.c_str(),reply.length(),0);
        std::cerr << "client "<< client.getUsername() << "! already authentified !" << std::endl;
        return;
    }
	if (command.command == "PING") {
        std::string reply = "PONG :server\r\n";
        send(client.getFd(), reply.c_str(), reply.length(), 0);
        // PRIVMSG(client, command);
		// joinHandler(channels, client, params);
		params.clear();
    }
    else if (command.command == "USER" && command.params.size() == 4) {
        client.USER(client.getFd(),clientBuffers, command.params[0]);
    }
    else if (command.command == "NICK" && command.params.size() == 1) {
        client.NICK(client.getFd(),clientBuffers, command.params[0]);
    }
    else if (command.command == "PRIVMSG") {
        PRIVMSG(client.getFd(),clientBuffers,client, command, channels);
    }
	else if (command.command == "JOIN")
		joinHandler(channels, client, params);
	else if (command.command == "TOPIC")
		topicHandler(channels, client, params);
	else {
        std::cerr << "Unknown command: " << command.command << std::endl;
    }
	params.clear();
}

void checkUniqueUsername(const std::string& username, const std::map<int, Client>& clientBuffers) {
    for (std::map<int, Client>::const_iterator it = clientBuffers.begin(); it != clientBuffers.end(); ++it) {
        if (it->second.getUsername() == username) {
            throw std::runtime_error("462 UserName: ERR_ALREADYREGISTERED");
        }
    }
}

void checkUniqueNickname(const std::string& nickname, const std::map<int, Client>& clientBuffers) {
    for (std::map<int, Client>::const_iterator it = clientBuffers.begin(); it != clientBuffers.end(); ++it) {
        if (it->second.getNickname() == nickname) {
            throw std::runtime_error("462 NickName: ERR_ALREADYREGISTERED");
        }
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