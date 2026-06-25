#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

void Server::handlePart(Client& client, Command& cmd)
{
    if (cmd.args.size() < 1)
    {
        std::string err = ":" + client.getNickname() + " 461 PART :Not enough parameters\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return ;
    }
    std::string channelName = cmd.args[0];
    std::string reason;
    if (cmd.args.size() > 1)
        reason = cmd.args[1];
    else
        reason = "Leaving";
    std::map<std::string, Channel>::iterator it;

    it = channels.find(channelName);
    if (it == channels.end())
    {
        std::string err = ":" + client.getNickname() + " 403 " + channelName + " :No such channel\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return ;
    }
    Channel &channel = it->second;
    if (!channel.hasMember(client.getFd()))
    {
        std::string err = ":" + client.getNickname() + " 442 " + channelName + " :You're not on that channel\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return ;
    }
    std::string msg = ":" + client.getNickname() + " PART " + channelName + " :" + reason + "\r\n";
    std::set<int> members = channel.getMembers();
    std::set<int>::iterator m = members.begin();
    while (m != members.end())
    {
        send(*m, msg.c_str(), msg.size(), 0);
        m++;
    }
    channel.removeMember(client.getFd());
    if (channel.memberCount() == 0)
        channels.erase(channelName);
}