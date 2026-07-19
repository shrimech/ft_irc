// #include "../client/client.hpp"
#include "../client/includes.hpp"
Server::Server(int port, std::string password) : _port(port), _serverSocket(-1), _serverName("irc.Brika.net"), _password(password) {}

std::string Server::getName() const {
    return _serverName;
}

Server::~Server() {
    for (size_t i = 0; i < _pollfds.size(); i++) {
        close(_pollfds[i].fd);
    }
    if (_serverSocket != -1) {
        close(_serverSocket);
    }
}

void Server::Init() {
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket < 0) throw std::runtime_error("Failed to create socket");

    int opt = 1;
    setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    fcntl(_serverSocket, F_SETFL, O_NONBLOCK);

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(_serverSocket, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("Failed to bind");

    if (listen(_serverSocket, SOMAXCONN) < 0)
        throw std::runtime_error("Failed to listen");

    struct pollfd srv;
    srv.fd = _serverSocket;
    srv.events = POLLIN;
    srv.revents = 0;
    _pollfds.push_back(srv);

    std::cout << "Server listening on port " << _port << std::endl;
}

void Server::Run() {
    while (true) {
        if (poll(&_pollfds[0], _pollfds.size(), -1) < 0)
            throw std::runtime_error("Poll failed");

        for (size_t i = 0; i < _pollfds.size(); i++) {
            if (_pollfds[i].revents == 0) 
                continue;

            if (_pollfds[i].revents & POLLIN) {
                if (_pollfds[i].fd == _serverSocket)
                    AcceptNewClient();
                else
                    ReceiveNewData(_pollfds[i].fd);
            }

            // disconnections
            if (_pollfds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {
                ClearClient(_pollfds[i].fd);
                i--; // cuz ClearClient() modify (pollfd) vect
            }
        }
    }
}

void Server::AcceptNewClient() {
    int client_fd = accept(_serverSocket, NULL, NULL);
    if (client_fd != -1) {
        fcntl(client_fd, F_SETFL, O_NONBLOCK);
        
        struct pollfd cli;
        cli.fd = client_fd;
        cli.events = POLLIN;
        cli.revents = 0;
        _pollfds.push_back(cli);
        
        std::cout << "New client connected: " << client_fd << std::endl;

        // Sending Welcome msg (ghir db smit client guest a si salah)
        std::string client_nickname = "Guest"; 
        
        std::string reply = ":" + this->getName() + " 001 " + client_nickname + " :Welcome to the Internet Relay Network you should authenticate to use our services\r\n";

        send(client_fd, reply.c_str(), reply.length(), 0);
    }
}

void Server::ReceiveNewData(int fd) {
    std::vector<char> buffer(1024, '\0');
    ssize_t bytes = recv(fd, &buffer[0], buffer.size() - 1, 0);

    if (bytes > 0) {
        _clientBuffers[fd].getCmd_line().append(&buffer[0], bytes);
        // // DEBUG: (for data lost)
        // std::cout << "[DEBUG] Client " << fd << " buffer size now: " 
        //     << _clientBuffers[fd].size() << std::endl;
        
        size_t pos;
        while ((pos = _clientBuffers[fd].getCmd_line().find("\r\n")) != std::string::npos) {
            std::cout << "Processing command from client " << fd << "   " << _clientBuffers[fd].getCmd_line() << std::endl;
            std::string command = _clientBuffers[fd].getCmd_line().substr(0, pos);

            _clientBuffers[fd].getCmd_line().erase(0, pos + 2);

            std::cout << "Command received from client " << fd << ": " << command << std::endl;
            
            if (!command.empty() && command[command.length() - 1] == '\r')
                command.erase(command.length() - 1);
            HandleCommand(fd, _password ,command);
        }
    } else if (bytes == 0) {
        std::cout << "Client " << fd << " disconnected." << std::endl;
        ClearClient(fd);
    }
}

// void Server::ClearClient(int fd) {
//     close(fd);
//     _clientBuffers.erase(fd);
    
//     // break is important (skip bug or segfault)
//     for (std::vector<struct pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); ++it) {
//         if (it->fd == fd) {
//             _pollfds.erase(it);
//             break;
//         }
//     }
// }

void Server::ClearClient(int fd) {
    close(fd);
    _clientBuffers.erase(fd);

    std::vector<struct pollfd>::iterator it = _pollfds.begin();
    
    while (it != _pollfds.end()) {
        if (it->fd == fd)
            it = _pollfds.erase(it);
        else
            ++it;
    }
}

/*
Line 01: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
Line 02: BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB
Line 03: CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
Line 04: DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
Line 05: EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
Line 06: FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
Line 07: GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG
Line 08: HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
Line 09: IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII
Line 10: JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ
Line 11: KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
Line 12: LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
Line 13: MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
Line 14: NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
Line 15: OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
Line 16: PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP
Line 17: QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ
Line 18: RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR
Line 19: SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
Line 20: TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
Line 21: UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU
Line 22: VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
Line 23: WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW
Line 24: XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
Line 25: YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY
Line 26: ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ
Line 27: 000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
Line 28: 111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111
Line 29: 222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222
Line 30: 333333333333333333333333333333333333333333333333333333333333333333333333333333333333333333
*/