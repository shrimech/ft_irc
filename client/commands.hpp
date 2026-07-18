#ifndef COMMANDS_PARSE_HPP
#define COMMANDS_PARSE_HPP

#include "client.hpp"

struct Command {
    std::string command;
    std::vector<std::string> params;
};

void parseCommand(const std::string& cmd_line, Command& command);
void executeCommands(Client& client, const Command& command);




#endif