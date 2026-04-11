#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

void    Server::handleJoin(Client& client, Command& cmd)
{
    if (cmd.args.size() < 1)
        return ;
    std::string channelName = cmd.args[0];
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
    std::cout << "Client " << client.getNickname() << " joined channel " << channelName << std::endl;
}

