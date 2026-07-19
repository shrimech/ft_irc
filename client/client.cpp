#include "includes.hpp"

Client::Client() : fd(-1)  {Authentification();}

Client::~Client() {}

void Client::setFd(int fd) {
    this->fd = fd;
}

int Client::getFd() const {
    return fd;
}

void Client::setCmd_line(const std::string& cmd_line) {
    command.cmd_line = cmd_line;
}

std::string& Client::getCmd_line()  {
    return command.cmd_line;
}

void Client::sendMessage(const std::string& message)
{
    size_t totalSent = 0;

    while (totalSent < message.size())
    {
        ssize_t bytes = send(fd, message.c_str() + totalSent, message.size() - totalSent, MSG_NOSIGNAL);

        if (bytes > 0)
        {
            totalSent += bytes;
        }
        else if (bytes == -1 && errno == EINTR)
        {
            continue;
        }
        else
        {
            std::cerr << "send() failed: " << strerror(errno) << std::endl;
            break;
        }
    }
}

