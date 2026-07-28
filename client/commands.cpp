#include "includes.hpp"
#include "../Bot/Bot.hpp"

void authentificate(Client& client,std::map<int, Client>& clientBuffers, const std::string& serv_pass, const std::string& commandLine) {
    Command command;
    parseCommand(commandLine, command);

    if (command.command == "NICK" && command.params.size() == 1) {
        client.NICK(client.getFd(),clientBuffers, command.params[0]); 
    } else if ((command.command == "USER") && command.params.size() == 4) {
        client.USER(client.getFd(),clientBuffers, command.params[0]);
    }
    else if ((command.command == "PASS") && client.getPassword() == serv_pass) {
        std::string reply = ":irc.Brika.net 462 * :You may not reregister\r\n";
        send(client.getFd(), reply.c_str(), reply.length(), 0);
    }
    else if ((command.command == "PASS") && command.params.size() == 1) {
        client.PASS(client.getFd(), command.params[0], serv_pass);
    }
    else if (command.command != "NICK" && command.command != "USER" && command.command != "PASS") {
        std::string reply = ":irc.Brika.net NOTICE AUTH :Please authenticate with PASS, NICK, and USER\r\n";
        send(client.getFd(), reply.c_str(), reply.length(), 0);
    }
    
	if (client.isAuthenticated()) {
		std::string nick = client.getNickname();
		std::string user = client.getUsername();
		std::string host = "localhost";

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
void executeCommands(Client& client, std::map<int, Client>& clientBuffers, const Command& command, ChannelRegistry& channels, QuizBot& bot);
void HandleCommand(int fd, std::map<int, Client>& clientBuffers, const std::string& serv_pass, const std::string& commandLine, ChannelRegistry& channels, QuizBot& bot) {
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
        executeCommands(client,clientBuffers, command, channels, bot);
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

void PRIVMSG(int fd, const std::map<int, Client>& clientBuffers, Client& client, const Command& command, ChannelRegistry& channels, QuizBot& bot) {
    
    // hadchi ana li zedto bash itparse l Bot
    if (command.params.size() == 2) {
        const std::string& target = command.params[0];
        
        if (target == "QuizBot" || target == "!bot" || target == bot.getName()) {
            std::string botMsg = command.params[1];
            std::string botReply = bot.handleCommand(fd, client.getNickname(), botMsg);
            send(fd, botReply.c_str(), botReply.length(), 0);
            std::cout << "[BOT] Replied to " << client.getNickname() << ": " << botMsg << std::endl;
            return;
        }
    }

    if (command.params.empty()) {
        std::string reply = "411 " + client.getNickname() + " :No recipient given (PRIVMSG)\r\n";
        send(fd, reply.c_str(), reply.length(), 0);
        return;
    }
    if (command.params.size() != 2) {
        std::string reply = "412 " + client.getNickname() + " :No text to send\r\n";
        send(fd, reply.c_str(), reply.length(), 0);
        return;
    }

    const std::string& recipient = command.params[0];
    const std::string& message = command.params[1];

    if (recipient.empty() || message.empty()) {
        std::string reply = "412 " + client.getNickname() + " :No text to send\r\n";
        send(fd,reply.c_str(),reply.length(),0);
        std::cerr << "PRIVMSG <recipient> <message>." << std::endl;
        return;
    }

    if (recipient[0] == '#') {
        if (!channels.channelExists(recipient)) {
            std::string reply = "403 " + client.getNickname() + " " + recipient + " :No such channel\r\n";
            send(fd, reply.c_str(), reply.length(), 0);
            std::cerr << "Channel " << recipient << " does not exist." << std::endl;
            return;
        }
        else {
            Channel* channel = channels.findChannel(recipient);
            if (channel) {
                if (!channel->isMember(client.getFd())) {
                    std::string reply = "404 " + client.getNickname() + " " + recipient + " :Cannot send to channel\r\n";
                    send(fd, reply.c_str(), reply.length(), 0);
                    std::cerr << "Client " << client.getFd() << " is not a member of channel " << recipient << "." << std::endl;
                    return;
                }
                std::string fullMessage = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PRIVMSG " + recipient + " :" + message + "\r\n";
                channel->broadCast(fullMessage, client.getFd());
                std::cout << "Message sent to channel " << recipient << ": " << message << std::endl;
                return;
            }
        }
        std::string reply = "404 " + client.getNickname() + " " + recipient + " :Cannot send to channel\r\n";
        send(fd, reply.c_str(), reply.length(), 0);
        std::cerr << "PRIVMSG to channels is not implemented yet." << std::endl;
        return;
    }


    // Find the recipient in the clientBuffers
    for (std::map<int, Client>::const_iterator pair = clientBuffers.begin(); pair != clientBuffers.end(); ++pair) {
        if(pair->second.getNickname() == recipient) {
            // Recipient found, send the message
            std::string fullMessage = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost PRIVMSG " + recipient + " :" + message + "\r\n";
            send(pair->second.getFd(), fullMessage.c_str(), fullMessage.length(), 0);
            std::cout << "Message sent to " << recipient << ": " << message << std::endl;
            return;
        }
    }

    // If recipient not found
    std::string errorReply = "401 " + client.getNickname() + " " + recipient + " :No such nick/channel\r\n";
    send(fd, errorReply.c_str(), errorReply.length(), 0);
    std::cerr << "Recipient " << recipient << " not found." << std::endl;
}


void executeCommands(Client& client, std::map<int, Client>& clientBuffers, const Command& command, ChannelRegistry& channels, QuizBot& bot) {
	std::vector<std::string> params = command.params;
    if(command.command == "PASS"|| command.command == "USER")
    {
        std::string reply =  "U're already authentified ! \r\n";
        send(client.getFd(),reply.c_str(),reply.length(),0);
        std::cerr << "client "<< client.getUsername() << "! already authentified !" << std::endl;
        return;
    }
    else if (command.command == "NICK" && command.params.size() == 1) {
        client.NICK(client.getFd(),clientBuffers, command.params[0]);
    }
    else if (command.command == "PRIVMSG") {
        PRIVMSG(client.getFd(),clientBuffers,client, command, channels, bot);
    }
	else if (command.command == "JOIN")
		joinHandler(channels, client, params);
	else if (command.command == "TOPIC")
		topicHandler(channels, client, params);
	else if (command.command == "INVITE")
		inviteHandler(channels, client, params, clientBuffers);
	else if (command.command == "KICK")
    	kickHandler(channels, clientBuffers, client, params);
	else if (command.command == "MODE")
    	modeHandler(channels, clientBuffers, client, params);
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
