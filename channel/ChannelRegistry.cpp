#include "ChannelRegistry.hpp"

ChannelRegistry::ChannelRegistry() {}

ChannelRegistry::~ChannelRegistry() {}

Channel* ChannelRegistry::findChannel(const std::string& name)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it == _channels.end())
		return NULL;
	return it->second;
}

Channel* ChannelRegistry::findOrCreate(const std::string& name)
{
	Channel* channel = findChannel(name);
	if(channel != NULL)
		return channel;
	channel = new Channel(name);
	_channels[name] = channel;
	return channel;
}

void ChannelRegistry::removeChannel(const std::string& name)
{
	std::map<std::string, Channel*>::iterator it = _channels.find(name);
	if (it == _channels.end())
		return;
	delete it->second;
	_channels.erase(it);
}

bool ChannelRegistry::channelExists(const std::string& name) const
{
	return _channels.find(name) != _channels.end();
}

void ChannelRegistry::removeClientFromAllChannels(int fd)
{
	std::map<std::string, Channel*>::iterator it = _channels.begin();
	while (it != _channels.end())
	{
		it->second->removeMember(fd);
		if (it->second->getMemberCount() == 0)
		{
			delete it->second;
			std::map<std::string, Channel*>::iterator toErase = it;
			it++;
			_channels.erase(toErase);
		}
		else
			it++;
	}
}
