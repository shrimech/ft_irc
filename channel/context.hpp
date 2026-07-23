#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "client/Client.hpp"
#include "channel/ChannelRegistry.hpp"
#include <sstream>

struct context
{
	std::string prefix;
	Client& client;
	ChannelRegistry& channels;
	// std::string cmd;
	std::vector<std::string> params;
	context(Client& c, ChannelRegistry& r, const std::vector<std::string>& a)
		:client(c), channels(r), params(a) {}
};


