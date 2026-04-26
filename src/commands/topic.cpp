#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

void Server::handleTopic(Client& client, Command& cmd)
{
    if (cmd.args.size() < 1)
        return ;
    std::string channelName = cmd.args[0];
    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it == channels.end())
        return ;
    Channel &channel = it->second;
    if (channel.hasMember(client.getFd()))
        return ;
    if (cmd.args.size() == 1)
    {
        std::string msg = ":" + channelName + " TOPIC :" + channel.getTopic() + "\r\n";
        send(client.getFd(), msg.c_str(), msg.size(), 0);
        return ;
    }
    if (channel.isTopicRestricted() && !channel.isOperator(client.getFd()))
        return ;
    std::string newTopic = cmd.args[1];
    channel.setTopic(newTopic);
    std::string msg = ":" + client.getNickname() + " TOPIC " + channelName + " :" + newTopic + "\r\n";
    std::set<int> members = channel.getMembers();
    std::set<int>::iterator m = members.begin();
    while (m != members.end())
    {
        send(*m, msg.c_str(), msg.size(), 0);
        m++;
    }
}