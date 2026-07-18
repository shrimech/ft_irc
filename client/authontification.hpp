#ifndef AUTHENTIFICATION_HPP
#define AUTHENTIFICATION_HPP

#include "./client.hpp"


class Authentification
{
private:
    std::string _password;
    std::string _nickname;
    std::string _username;
    bool        _authenticated;

public:
    Authentification();
    ~Authentification();
    
    std::string getPassword() const;
    void setPassword(const std::string &password);
    
    std::string getNickname() const;
    void setNickname(const std::string &nickname);
    
    std::string getUsername() const;
    void setUsername(const std::string &username);

    bool isAuthenticated() const;
    void setAuthenticated(bool authenticated);
};


#endif