#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "includes.hpp"
// #include "commands.hpp"
// class message;
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


class message
{
    protected:
        std::string _content;
        std::string _recipient;
        int         _server_fd;

    public:
        message(const std::string &content, const std::string &sender, const std::string &recipient);
        const std::string &getContent() const;
        const std::string &getSender() const;
        const std::string &getRecipient() const;
        void setContent(const std::string &content);
        void setSender(const std::string &sender);
        void setRecipient(const std::string &recipient);

};
#endif