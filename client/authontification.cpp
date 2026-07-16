#include "authontification.hpp"

Authentification::Authentification() : _authenticated(false) {}

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

void Authentification::setAuthenticated(bool authenticated) {
    _authenticated = authenticated;
}