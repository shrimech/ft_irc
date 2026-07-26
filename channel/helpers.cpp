#include "ChannelCommands.hpp"


void replyMsg(const std::string& rcode, const std::string& msg, Client& client)
{
	// prefix = servername / ( nickname [ [ "!" user ] "@" host ] )
	std::string nickName = client.getNickname();
	client.sendMessage(":irc.1337.brika" + rcode + nickName + " " + msg  + "\r\n");
}

std::string clientPrefix(Client& client) {
    return ":" + client.getNickname() + "!" + client.getUsername() + "@localhost";
}

Client* findClientByNick(std::map<int, Client>& clients, const std::string& nick)
{
	for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); it++)
	{
		if (it->second.getNickname() == nick)
			return &it->second;
	}
	return NULL;
}