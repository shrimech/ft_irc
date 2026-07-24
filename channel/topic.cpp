#include "ChannelCommands.hpp"

void topicHandler(context &ctx)
{
	if (ctx.params.empty())
		return replyMsg(ctx, " 461 ", "TOPIC :Not enough parameters");
	std::string channelName = ctx.params[0];
	if (channelName.size() < 2 || (channelName[0] != '#' && channelName[0] != '&'))
		return replyMsg(ctx, " 403 ", channelName + " :No such channel");
	Channel* channel = ctx.channels.findChannel(channelName);
	if (!channel)
		return replyMsg(ctx, " 403 ", channelName + " :No such channel");
	int fd = ctx.client.getFD();
	if (!channel->isMember(fd))
		return replyMsg(ctx, " 442 ", channelName + " :You're not on that channel");
	if (ctx.params.size() < 2)
	{
		if (!channel->hasTopic())
			return replyMsg(ctx, " 331 ", channelName + " :No topic is set.");
		else
			return replyMsg(ctx, " 332 ", channelName + " :" + channel->getTopic());
		// 333
	}
	if (channel->isTopicRestricted() && !channel->isOperator(fd))
		return replyMsg(ctx, " 482 ", channelName + " :You're not a channel operator");
	channel->setTopic(ctx.params[1]);
	std::string msg = ctx.client.getPrefix() + " TOPIC " + channelName + " :" + channel->getTopic();
	channel->broadCast(msg);
}
