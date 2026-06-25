#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

void Server::handleInvite(Client &client, Command &cmd)
{
    if (cmd.args.size() < 2)
    {
        std::string err = ":" + client.getNickname() + " 461 INVITE :Not enough parameters\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return;
    }
    std::string nickname = cmd.args[0];
    std::string channelName = cmd.args[1];
    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it == channels.end())
    {
        std::string err = ":" + client.getNickname() + " 403 " + channelName + " :No such channel\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return;
    }
    Channel &channel = it->second;
    if (!channel.hasMember(client.getFd()))
    {
        std::string err = ":" + client.getNickname() + " 442 " + channelName + " :You're not on that channel\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return;
    }
    if (!channel.isOperator(client.getFd()))
    {
        std::string err = ":" + client.getNickname() + " 482 " + channelName + " :You're not channel operator\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return;
    }
    Client *target = getClientByNick(nickname);
    if (!target)
    {
        std::string err = ":" + client.getNickname() + " 401 " + nickname + " :No such nick\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return;
    }
    if (channel.hasMember(target->getFd()))
    {
        std::string err = ":" + client.getNickname() + " 443 " + nickname + " " + channelName + " :is already on channel\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return;
    }
    channel.addInvite(target->getFd());
    std::string msg = ":" + client.getNickname() + " INVITE " + nickname + " :" + channelName + "\r\n";
    send(target->getFd(), msg.c_str(), msg.size(), 0);
}