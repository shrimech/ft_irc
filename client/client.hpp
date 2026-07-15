#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class message{
    private:
        std::string _content;
        std::string _sender;
        std::string _recipient;
        int         _server_fd;

    public:
        message(const std::string &content, const std::string &sender, const std::string &recipient);
        const std::string &getContent() const;
        const std::string &getSender() const;
        const std::string &getRecipient() const;
        void sendMessage(const std::string &message);
};

class Client
{
private:
    std::string _nickname;
    std::string _username;
    std::string _realname;
    bool        _authenticated;
    std::vector<int>        _clients_fd;
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

    void joinChannel(const std::string &channel);
    void leaveChannel(const std::string &channel);
};


#endif