#include "./client.hpp"
#include <sys/socket.h>
#include <cstring>

Client::Client() {}

Client::~Client() {}

void Client::setFd(int fd) {
    this->fd = fd;
}

int Client::getFd() const {
    return fd;
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

