#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include "../client/client.hpp"

class Server {
private:
    int _port;
    int _serverSocket;
    std::string _serverName;
    std::string _password;
    std::vector<struct pollfd> _pollfds;
    std::map<int, std::string> _clientBuffers; // atwali std::map<int, Client> (a si salah)

    void AcceptNewClient();
    void ReceiveNewData(int fd);
    void ClearClient(int fd);

public:
    Server(int port, std::string password);
    std::string getName() const;
    ~Server();

    void Init(); // Set up socket, bind, listen
    void Run();  // poll() loop
};

#endif