#ifndef COMMANDS_PARSE_HPP
#define COMMANDS_PARSE_HPP

#include "client.hpp"

struct Command {
    std::string cmd_line;
    std::string command;
    std::vector<std::string> params;
};




#endif