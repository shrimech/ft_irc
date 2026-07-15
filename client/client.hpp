#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>



class Client
{
private:
    std::string _nickname;
    std::string _username;
    std::string _realname;
    std::vector<std::string> _channels;

public:
    Client();
    ~Client();

    void setNickname(const std::string &nickname);
    void setUsername(const std::string &username);
    void setRealname(const std::string &realname);

    const std::string &getNickname() const;
    const std::string &getUsername() const;
    const std::string &getRealname() const;

    void sendMessage(const std::string &message);
    void joinChannel(const std::string &channel);
    void leaveChannel(const std::string &channel);
};


#endif