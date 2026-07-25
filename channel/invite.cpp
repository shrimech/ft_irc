#include "ChannelCommands.hpp"

// void inviteHandler(ChannelRegistry& channels, Client& client, std::vector<std::string> params)
// {
// 	if (params.size() < 2)
// 		return replyMsg()
// }

// void inviteHandler(context& ctx)
// {
// 	if (ctx.params.size() < 2)
// 		return replyMsg(ctx, " 461 ", "INVITE :Not enough parameters");
// 	std::string targetNick = ctx.params[0];
// 	std::string channelName = ctx.params[1];
// 	if (channelName.size() < 2 || (channelName[0] != '#' && channelName[0] != '&'))
// 		return replyMsg(ctx, " 403 ", channelName + " :No such channel");
// 	Channel* channel = ctx.channels.findChannel(channelName);
// 	if (!channel)
// 		return replyMsg(ctx, " 403 ", channelName + " :No such channel");
// 	int fd = ctx.client.getFD();
// 	if (!channel->isMember(fd))
// 		return replyMsg(ctx, " 442 ", channelName + " :You're not on that channel");
// 	// NOTE: findClientByNick assumed to exist on Server, owned by Achraf.
// 	// Unconfirmed as of now.
// 	Client* target = ctx.server.findClientByNick(targetNick);
// 	if (!target)
// 		return replyMsg(ctx, " 401 ", targetNick + " :No such nick/channel");
// 	if (channel->isMember(target->getFD()))
// 		return replyMsg(ctx, " 443 ", targetNick + " " + channelName + " :is already on channel");
// 	if (channel->isInviteOnly() && !channel->isOperator(fd))
// 		return replyMsg(ctx, " 482 ", channelName + " :You're not a channel operator");
// 	channel->addInvite(target->getFD());
// 	target->sendMessage(ctx.client.getPrefix() + " INVITE " + targetNick + " :" + channelName);
// 	// RFC 2812 text lists args as "<channel> <nick>", but RFC Errata ID 2822
// 	// (verified) confirms real servers/clients send "<nick> <channel>".
// 	// Using errata order since real-server behavior is your compliance target.
// 	replyMsg(ctx, " 341 ", targetNick + " " + channelName);
// }
