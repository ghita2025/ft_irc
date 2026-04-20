#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

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
    if (channel.isInviteOnly() && !channel.isInvited(client.getFd()))
        return ;
    if (channel.hasKey())
    {
        if (cmd.args.size() < 2)
            return;
        std::string key = cmd.args[1];
        if (key != channel.getKey())
            return;
    }
    if (channel.hasMember(client.getFd()))
        return ;
    channel.addMember(client.getFd());
    channel.removeInvite(client.getFd());
    if (channel.memberCount() == 1)
        channel.addOperator(client.getFd());
    client.joinChannel(channelName);
    std::string msg = ":" + client.getNickname() + " JOIN " + channelName + "\r\n";
    std::set<int> members = channel.getMembers();
    std::set<int>::iterator it = members.begin();
    while (it != members.end())
    {
        send(*it, msg.c_str(), msg.size(), 0);
        it++;
    }
}

