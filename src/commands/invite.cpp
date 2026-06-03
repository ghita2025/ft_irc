#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

void Server::handleInvite(Client &client, Command &cmd)
{
    if (cmd.args.size() < 2)
        return;
    std::string nickname = cmd.args[0];
    std::string channelName = cmd.args[1];
    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it == channels.end())
        return;
    Channel &channel = it->second;
    if (!channel.hasMember(client.getFd()))
        return;
    if (!channel.isOperator(client.getFd()))
        return;
    Client *target = getClientByNick(nickname);
    if (!target)
        return;
    if (channel.hasMember(target->getFd()))
        return;
    channel.addInvite(target->getFd());
    std::string msg = ":" + client.getNickname() + " INVITE " + nickname + " :" + channelName + "\r\n";
    send(target->getFd(), msg.c_str(), msg.size(), 0);
}