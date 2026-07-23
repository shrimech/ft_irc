#include "channel/Channel.hpp"
#include "client/Client.hpp"
#include "channel/ChannelRegistry.hpp"
#include "context.hpp"
#include "channel/ChannelCommands.hpp"


int main()
{
	Client client;
	ChannelRegistry channels;
	std::vector<std::string> params;
	std::stringstream ss;
	std::string line, cmd, param;
	while (true)
	{
		std::getline(std::cin, line);
		ss << line;
		ss >> cmd;
		while (ss >> param)
			params.push_back(param);
		context ctx = context(client, channels, params);
		joinHandler(ctx);
		ss.clear();
		cmd.clear();
		params.clear();
		std::cout << "\n";
	}
}
