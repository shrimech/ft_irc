#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "includes.hpp"



void authentificate(Client& client,std::map<int, Client>& clientBuffers, const std::string& serv_pass, const std::string& commandLine);
void executeCommands(Client& client, const Command& command);
void HandleCommand(int fd,std::map<int, Client>& clientBuffers, const std::string& serv_pass, const std::string& commandLine);
void parseCommand(const std::string& cmd_line, Command& command);
void checkUniqueUsername(const std::string& username, const std::map<int, Client>& clientBuffers);


// ---------- commands ------------------------------
void PRIVMSG(Client& client,const Command& command);

#endif