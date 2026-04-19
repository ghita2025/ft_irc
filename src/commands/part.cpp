#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

void Server::handlePart(Client& client, Command& cmd)
{
    if (cmd.args.size() < 1)
        return ;
    std::string channelName = cmd.args[0];
    std::string reason;
    if (cmd.args.size() > 1)
        reason = cmd.args[1];
    else
        reason = "Leaving";
    std::map<std::string, Channel>::iterator it;

    it = channels.find(channelName);
    if (it == channels.end())
        return ;
    Channel &channel = it->second;
    if (!channel.hasMember(client.getFd()))
        return ;
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