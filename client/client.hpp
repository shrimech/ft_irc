#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include "./authontification.hpp"

using namespace std;

class message{
    private:
        std::string _content;
        std::string _sender;
        std::string _recipient;
        int         _server_fd;

    public:
        message(const std::string &content, const std::string &sender, const std::string &recipient);
        const std::string &getContent() const;
        const std::string &getSender() const;
        const std::string &getRecipient() const;
        void sendMessage(const std::string &message);
};

class Client : public Authentification 
{
private:
    int fd;

public:
    Client();
    ~Client();

    void setFd(int fd);
    int getFd() const;

    void sendMessage(const std::string &message);
};


#endif