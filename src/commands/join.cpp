#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"
#include <sys/socket.h>

void    Server::handleJoin(Client& client, Command& cmd)
{
    if (cmd.args.size() < 1)
        return ;
    std::string channelName = cmd.args[0];
    if (channelName[0] != '#')
        return ;
    if (channels.find(channelName) == channels.end())
    {
        Channel newChannel(channelName);
        channels[channelName] = newChannel;
    }
    Channel &channel = channels[channelName];
    if (channel.hasMember(client.getFd()))
        return ;
    channel.addMember(client.getFd());
    if (channel.memberCount() == 1)
        channel.addOperator(client.getFd());
    std::string msg = ":" + client.getNickname() + " JOIN " + channelName + "\r\n";
    std::set<int>::const_iterator it = channel.getMembers().begin();
    while (it != channel.getMembers().end())
    {
        send(*it, msg.c_str(), msg.size(), 0);
        it++;
    }
}

