#include "./client.hpp"

Client::Client() {}

Client::~Client() {}


void Client::joinChannel(const std::string &channel) {
    _channels.push_back(channel);
}

void Client::leaveChannel(const std::string &channel) {
    for (auto it = _channels.begin(); it != _channels.end(); ++it) {
        if (*it == channel) {
            _channels.erase(it);
            break;
        }
    }
}
