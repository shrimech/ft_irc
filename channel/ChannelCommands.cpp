#include "ChannelCommands.hpp"
#include "ChannelRegistry.hpp"

void replyMsg(context& ctx, const std::string& rcode, const std::string& msg)
{
	// prefix = servername / ( nickname [ [ "!" user ] "@" host ] )
	std::string nickName = ctx.client.getNickname();
	ctx.client.sendMessage(":irc.1337.brika" + rcode + nickName + " " + msg);
}

void joinHandler(context& ctx)
{
	if (ctx.params.empty())
		return replyMsg(ctx, " 461 ", "JOIN :Not enough parameters");
	std::string channelName = ctx.params[0];
	if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&'))
		return replyMsg(ctx, " 403 ", channelName + " :No such channel");
	Channel *channel = ctx.channels.findOrCreate(channelName);
	int fd = ctx.client.getFD();
	if (channel->isMember(fd))
		return;
	if (channel->hasUserLimit() && static_cast<size_t>(channel->getUserLimit()) <= channel->getMemberCount())
		return replyMsg(ctx, " 471 ", channelName + " :Cannot join channel (+l)");
	if (channel->isInviteOnly() && !channel->isInvited(fd))
		return replyMsg(ctx, " 473 ", channelName + " :Cannot join channel (+i)");
	if (channel->hasKey())
	{
		std::string key = ctx.params.size() > 1 ? ctx.params[1] : "";
		if (key != channel->getKey())
			return replyMsg(ctx, " 475 ", channelName + " :Cannot join channel (+k)");
	}
	if (channel->getMemberCount() == 0)
		channel->addOperator(fd);
	channel->addMember(fd, &ctx.client);
	std::string msg = ctx.client.getPrefix() + " JOIN " + channelName;
	channel->broadCast(msg);
	replyMsg(ctx, " 353 ", "= " + channelName + " :" + channel->getMembersName());
	replyMsg(ctx, " 366 ", channelName + " :End of /Names list");
}
