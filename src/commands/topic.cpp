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
    if (cmd.args.size() == 1)
    {
        if (channel.getTopic().empty())
        {
            std::string reply = ":" + client.getNickname() + " 331 " + channelName + " :No topic is set\r\n";
            send(client.getFd(), reply.c_str(), reply.size(), 0);
        }
        else
        {
            std::string reply = ":" + client.getNickname() + " 332 " + channelName + " :" + channel.getTopic() + "\r\n";
            send(client.getFd(), reply.c_str(), reply.size(), 0);
        }
        return ;
    }
    if (channel.isTopicRestricted() && !channel.isOperator(client.getFd()))
    {
        std::string err = ":" + client.getNickname() + " 482 " + channelName + " :You're not channel operator\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return ;
    }
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