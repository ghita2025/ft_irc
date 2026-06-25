#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

void Server::handleJoin(Client &client, Command &cmd)
{
    if (cmd.args.size() < 1)
    {
        std::string err = ":" + client.getNickname() + " 461 JOIN :Not enough parameters\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return;
    }
    std::string channelName = cmd.args[0];
    if (channelName[0] != '#')
    {
        std::string err = ":" + client.getNickname() + " 476 " + channelName + " :Bad Channel Mask\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return ;
    }
    if (channels.find(channelName) == channels.end())
    {
        Channel newChannel(channelName);
        channels[channelName] = newChannel;
    }
    Channel &channel = channels[channelName];
    if (channel.isInviteOnly() && !channel.isInvited(client.getFd()))
    {
        std::string err = ":" + client.getNickname() + " 473 " + channelName + " :Invite only channel\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return;
    }
    if (channel.hasPassword())
    {
        if (cmd.args.size() < 2 || cmd.args[1] != channel.getPassword())
        {
            std::string err = ":" + client.getNickname() + " 475 " + channelName + " :Cannot join channel (+k)\r\n";
            send(client.getFd(), err.c_str(), err.size(), 0);
            return;
        }
    }
    if (channel.isFull())
    {
        std::string err = ":" + client.getNickname() + " 471 " + channelName + " :Cannot join channel (+l)\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return;
    }
    if (channel.isInviteOnly())
        std::cout << "Channel is invite only" << std::endl;
    if (channel.hasMember(client.getFd()))
        return;
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
