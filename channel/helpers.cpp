#include "ChannelCommands.hpp"


void replyMsg(const std::string& rcode, const std::string& msg, Client& client)
{
	// IRC numerics are sent with a server prefix so clients recognize them as
	// server-generated replies rather than user messages.
	std::string nickName = client.getNickname();
	client.sendMessage(":irc.Brika.net" + rcode + nickName + " " + msg  + "\r\n");
}

std::string clientPrefix(Client& client) {
	// The host part is still a local placeholder here. Replace it with the real
	// peer address if you want fully accurate IRC prefixes.
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