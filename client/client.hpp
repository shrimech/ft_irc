#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "includes.hpp"

struct Command {
    std::string cmd_line;
    std::string command;
    std::vector<std::string> params;
};
class Client : public Authentification
{
    protected:
        int fd;
        struct Command command;
        
    public:
        Client();
        ~Client();

        void setFd(int fd);
        int getFd() const;
        void setCmd_line(const std::string& cmd_line);
    
        std::string& getCmd_line() ;
        std::vector<std::string>& getParams();
        std::string& getCommand();
        void sendMessage(const std::string &message);
};

#endif