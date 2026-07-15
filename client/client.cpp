#include "./client.hpp"

Client::Client() {}

Client::~Client() {}

void Client::setNickname(const std::string &nickname) {
    _nickname = nickname;
}

void Client::setUsername(const std::string &username) {
    _username = username;
}

void Client::setRealname(const std::string &realname) {
    _realname = realname;
}

const std::string &Client::getNickname() const {
    return _nickname;
}

const std::string &Client::getUsername() const {
    return _username;
}

const std::string &Client::getRealname() const {
    return _realname;
}

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

void Client::sendMessage(const std::string &message) {
    
}
