#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

void Server::handleJoin(Client &client, Command &cmd)
{
    std::string serverName = "ircserv";
    if (cmd.args.size() < 1)
    {
        std::string err = ":" + serverName + " 461 " + client.getNickname() + " JOIN :Not enough parameters\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return;
    }
    std::string channelName = cmd.args[0];
    std::cout << "name channel: " << channelName[0] << std::endl;
    if (channelName.empty() || channelName[0] != '#')
    {
        std::string err = ":" + serverName +
        " 476 " + client.getNickname() +
        " " + channelName +
        " :Bad Channel Mask\r\n";
        std::cout << err << std::endl;
        ssize_t n = send(client.getFd(), err.c_str(), err.size(), 0);
        std::cout << "send returned = " << n << std::endl;
        perror("send");
        return;
    }
    if (channels.find(channelName) == channels.end())
    {
        Channel newChannel(channelName);
        channels[channelName] = newChannel;
    }
    Channel &channel = channels[channelName];
    if (channel.isInviteOnly() && !channel.isInvited(client.getFd()))
    {
        std::string err = ":" + serverName + " 473 " + client.getNickname() + " " + channelName + " :Invite only channel\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return;
    }
    if (channel.hasPassword())
    {
        if (cmd.args.size() < 2 || cmd.args[1] != channel.getPassword())
        {
            std::string err = ":" + serverName + " 475 " + client.getNickname() + " " + channelName + " :Cannot join channel (+k)\r\n";
            send(client.getFd(), err.c_str(), err.size(), 0);
            return;
        }
    }
    if (channel.isFull())
    {
        std::string err = ":" + serverName + " 471 " + client.getNickname() + " " + channelName + " :Cannot join channel (+l)\r\n";
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
    std::string listnames = "";
    std::set<int>::iterator it2 = members.begin();
    while (it2 != members.end())
    {
        Client mClient = clients[*it2];

        // std::cout << "names client: " + mClient.getNickname() << std::endl;
        if (channel.isOperator(mClient.getFd()))
            listnames += "@" + mClient.getNickname() + " ";
        else
            listnames += mClient.getNickname() + " ";
        it2++;
    }
    std::string namesMsg = ":" + serverName + " 353 " + client.getNickname() + " = " + channelName + " :" + listnames + "\r\n";
    send(client.getFd(), namesMsg.c_str(), namesMsg.size(), 0);
    std::string endNamesMsg = ":" + serverName + " 366 " + client.getNickname() + " " + channelName + " :End of /NAMES list\r\n";
    send(client.getFd(), endNamesMsg.c_str(), endNamesMsg.size(), 0);
}
