#include "includes.hpp"

Authentification::Authentification() : _authenticated(false) , _password(""), _nickname(""), _username("") {}

Authentification::~Authentification() {}
//--------------------------------------------------------

std::string Authentification::getPassword() const {
    return _password;
}

void Authentification::setPassword(const std::string &password) {
    _password = password;
}
//--------------------------------------------------------

std::string Authentification::getNickname() const {
    return _nickname;
}

void Authentification::setNickname(const std::string &nickname) {
    _nickname = nickname;
}
//--------------------------------------------------------

std::string Authentification::getUsername() const {
    return _username;
}

void Authentification::setUsername(const std::string &username) {
    _username = username;
}
//--------------------------------------------------------
bool Authentification::isAuthenticated() const {
    return _authenticated;
}

void Authentification::setAuthenticated() {
    if (!_nickname.empty() && !_username.empty() && !_password.empty()) {
        _authenticated = true;
    } else {
        _authenticated = false;
    }
}
// ------------- authentication commands ---------------------

void Authentification::NICK(int fd, const std::string &nickname) {
    setNickname(nickname);
    std::string reply = "Nickname set to: " + nickname + "\r\n";
    send(fd, reply.c_str(), reply.length(), 0);
    setAuthenticated();
}

void Authentification::USER(int fd,std::map<int, Client>& clientBuffers, const std::string &username) {
    try {
        checkUniqueUsername(username, clientBuffers);
    } catch (const std::runtime_error& e) {
        std::string reply = "Error: " + std::string(e.what()) + "\r\n";
        send(fd, reply.c_str(), reply.length(), 0);
        return;
    }
    setUsername(username);
    std::string reply = "Username set to: " + username + "\r\n";
    send(fd, reply.c_str(), reply.length(), 0);
    setAuthenticated();
}

void Authentification::PASS(int fd, const std::string &password, const std::string &serv_pass) {
    // std::cout << "{DEBUG}: User:?????????????????????????????? " << serv_pass << "====================== "<< password << " is trying to authenticate." << std::endl;
    if (password == serv_pass) {
        _password = password;
        setAuthenticated();
    } else {
        std::cout << "User: " << fd << " set an Incorrect password or try to brute force the server." << std::endl;
    }
    if (isAuthenticated() && _password == serv_pass) {
        std::string reply = "Password accepted. You are now authenticated.\r\n";
        send(fd, reply.c_str(), reply.length(), 0);
    }
    else if (!isAuthenticated() && _password == serv_pass) {
        std::string reply = "password Accepted, continue ur authentication process.\r\n";
        send(fd, reply.c_str(), reply.length(), 0);
    } else {
        std::string reply = "Incorrect password. Authentication failed, Enter the correct irc.Brika.net's password.\r\n";
        send(fd, reply.c_str(), reply.length(), 0);
    }
}