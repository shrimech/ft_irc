#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include "../client/includes.hpp"
#include "../channel/ChannelRegistry.hpp"
#include "../Bot/Bot.hpp"

// class QuizBot;
class Server {
private:
    int _port;
    int _serverSocket;
    std::string _serverName;
    std::string _password;
    std::vector<struct pollfd> _pollfds;
    std::map<int, Client> _clientBuffers;
    QuizBot _bot;

    void AcceptNewClient();
    void ReceiveNewData(int fd, ChannelRegistry& channels);
    void ClearClient(int fd);

public:
    Server(int port, std::string password);
    std::string getName() const;
    ~Server();

    void Init();
    void Run();
};



#endif