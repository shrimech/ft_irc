#pragma once

#include "Channel.hpp"
#include "ChannelRegistry.hpp"
#include <vector>

class Client;
class ChannelRegistry;

std::string clientPrefix(Client& client);
void replyMsg(const std::string& rcode, const std::string& msg, Client& client);
void joinHandler(ChannelRegistry& channels, Client& client, std::vector<std::string> params);
void topicHandler(ChannelRegistry& channels, Client& client, std::vector<std::string> params);
void inviteHandler(ChannelRegistry& channels, Client& client, std::vector<std::string> params);
