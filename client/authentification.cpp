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

static std::string makeUniqueUsername(int fd, const std::string& username, const std::map<int, Client>& clientBuffers) {
    std::string candidate = username;

    while (true) {
        bool taken = false;
        for (std::map<int, Client>::const_iterator it = clientBuffers.begin(); it != clientBuffers.end(); ++it) {
            if (it->first == fd)
                continue;
            if (it->second.getUsername() == candidate) {
                taken = true;
                break;
            }
        }
        if (!taken)
            return candidate;
        candidate += "_";
    }
}

static std::string makeUniqueNickname(int fd, const std::string& nickname, const std::map<int, Client>& clientBuffers) {
	std::string candidate = nickname;

	while (true) {
		bool taken = false;
		for (std::map<int, Client>::const_iterator it = clientBuffers.begin(); it != clientBuffers.end(); ++it) {
			if (it->first == fd)
				continue;
			if (it->second.getNickname() == candidate) {
				taken = true;
				break;
			}
		}
		if (!taken)
			return candidate;
		candidate += "_";
	}
}

void Authentification::NICK(int fd, std::map<int, Client>& clientBuffers, const std::string &nickname) {
    std::string uniqueNickname = makeUniqueNickname(fd, nickname, clientBuffers);

    Client& self = clientBuffers[fd];
    std::string oldPrefix = ":" + self.getNickname() + "!" + self.getUsername() + "@localhost";

    setNickname(uniqueNickname);

    if (isAuthenticated()) {
        std::string msg = oldPrefix + " NICK :" + uniqueNickname + "\r\n";
        self.sendMessage(msg);
        return;
    }

    std::string reply = ":irc.Brika.net NOTICE * :Nickname set to " + uniqueNickname + "\r\n";
    self.sendMessage(reply);

    setAuthenticated();
    if (!isAuthenticated()) {
        std::string reply2 = ":irc.Brika.net NOTICE * :Continue with authentication (NICK/USER/PASS)\r\n";
        self.sendMessage(reply2);
    }
}

void Authentification::USER(int fd,std::map<int, Client>& clientBuffers, const std::string &username) {
    std::string uniqueUsername = makeUniqueUsername(fd, username, clientBuffers);
    setUsername(uniqueUsername);
    if (isAuthenticated()) {
        std::string reply = "you are now known as: " + uniqueUsername + "\r\n";
        send(fd, reply.c_str(), reply.length(), 0);
        return;
    }
    std::string reply = "Username set to: " + uniqueUsername + "\r\n";
    send(fd, reply.c_str(), reply.length(), 0);
    setAuthenticated();
    if(!isAuthenticated()) {
        std::string reply = "Continue with the authentication process NICK/USER/PASS.\r\n";
        send(fd, reply.c_str(), reply.length(), 0);
    }
}

void Authentification::PASS(int fd, const std::string &password, const std::string &serv_pass) {
    if (password == serv_pass) {
        _password = password;
        setAuthenticated();
    }
    if (isAuthenticated() && _password == serv_pass) {
        std::string reply = "Password accepted. You are now authenticated.\r\n";
        send(fd, reply.c_str(), reply.length(), 0);
    }
    else if (!isAuthenticated() && _password == serv_pass) {
        std::string reply = "password Accepted, continue ur authentication process.\r\n";
        send(fd, reply.c_str(), reply.length(), 0);
    } else {
        std::string reply = "464 ERR_PASSWDMISMATCH\r\n";
        std::cout << "User: " << fd << " set an Incorrect password or try to brute force the server." << std::endl;
        send(fd, reply.c_str(), reply.length(), 0);
    }
            
}

