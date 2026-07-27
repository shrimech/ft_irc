#include "ChannelCommands.hpp"
#include <sstream>

static std::string currentModeString(Channel* channel)
{
	std::string modes = "+";
	if (channel->isInviteOnly())
		modes += "i";
	if (channel->isTopicRestricted())
		modes += "t";
	if (channel->hasKey())
		modes += "k";
	if (channel->hasUserLimit())
		modes += "l";
	return modes;
}

void modeHandler(ChannelRegistry& channels, std::map<int, Client>& clients, Client& client, std::vector<std::string> params)
{
	if (params.empty())
		return replyMsg(" 461 ", "MODE :Not enough parameters", client);
	std::string channelName = params[0];
	if (channelName.size() < 2 || (channelName[0] != '#'))
		return ;
	Channel* channel = channels.findChannel(channelName);
	if (!channel)
		return replyMsg(" 403 ", channelName + " :No such channel", client);
	int fd = client.getFd();
	if (!channel->isMember(fd))
		return replyMsg(" 442 ", channelName + " :You're not on that channel", client);
	if (params.size() < 2)
		return replyMsg(" 324 ", channelName + " " + currentModeString(channel), client);
	if (!channel->isOperator(fd))
		return replyMsg(" 482 ", channelName + " :You're not a channel operator", client);
	std::string modeString = params[1];
	size_t nextArg = 2;
	bool adding = true;
	std::string addedFlags, addedParams;
	std::string removedFlags, removedParams;
	for (size_t i = 0; i < modeString.size(); i++) {
		char c = modeString[i];
		if (c == '+' || c == '-') {
			adding = (c == '+');
			continue;
		}
		switch (c) {
			case 'i':
				channel->setInvitedOnly(adding);
				(adding ? addedFlags : removedFlags) += c;
				break;
			case 't':
				channel->setTopicRestricted(adding);
				(adding ? addedFlags : removedFlags) += c;
				break;
			case 'k':
				if (adding) {
					if (nextArg >= params.size())
						break;
					channel->setKey(params[nextArg]);
					addedFlags += c;
					addedParams += " " + params[nextArg];
					nextArg++;
				}
				else {
					channel->removeKey();
					removedFlags += c;
				}
				break;
			case 'l':
				if (adding) {
					if (nextArg >= params.size())
						break;
					std::istringstream iss(params[nextArg]);
					int limit = 0;
					iss >> limit;
					channel->setUserLimit(limit);
					addedFlags += c;
					addedParams += " " + params[nextArg];
					nextArg++;
				}
				else {
					channel->removeUserLimit();
					removedFlags += c;
				}
				break;
			case 'o': {
					if (nextArg >= params.size())
						break;
					Client* target = findClientByNick(clients, params[nextArg]);
					if (!target || !channel->isMember(target->getFd())) {
						replyMsg(" 441 ", params[nextArg] + " " + channelName + " :They aren't on that channel", client);
						nextArg++;
						break;
					}
					if (adding) {
						channel->addOperator(target->getFd());
						addedFlags += c;
						addedParams += " " + params[nextArg];
					}
					else {
						channel->removeOperator(target->getFd());
						removedFlags += c;
						removedParams += " " + params[nextArg];
					}
					nextArg++;
					break;
				}
			default:
				replyMsg(" 472 ", std::string(1, c) + " :is unknown mode char to me for " + channelName, client);
				break;
		}
	}
	if (addedFlags.empty() && removedFlags.empty())
		return;
	std::string modeChange;
	if (!addedFlags.empty())
		modeChange += "+" + addedFlags;
	if (!removedFlags.empty())
		modeChange += "-" + removedFlags;
	std::string allParams = addedParams + removedParams;
	std::string msg = clientPrefix(client) + " MODE " + channelName + " " + modeChange + allParams + "\r\n";
	channel->broadCast(msg);
}