#ifndef BOT_HPP
#define BOT_HPP

#include "../Server/Server.cpp"


class Bot {
    private:
        Server& _server;
    public:
        Bot(Server& server) : _server(server) {}
        ~Bot() {}
        void connectToServer(const std::string& host, int port);
        void sendMessage(const std::string& message);
};


#endif