#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

std::vector<std::string> split(const std::string &str, char c)
{
    std::vector<std::string> r;
    std::stringstream ss(str);
    std::string t;

    while (std::getline(ss, t, c))
    {
        r.push_back(t);
    }
    return (r);
}

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
    std::string strkey;
    if (cmd.args.size() >= 2)
        strkey = cmd.args[1];
    std::cout << "name channel: " << channelName << std::endl;
    std::vector<std::string> splitchannelname = split(channelName, ',');
    std::vector<std::string> splitkey;
    if (!strkey.empty())
        splitkey = split(strkey, ',');
    size_t i = 0;
    while (i < splitchannelname.size())
    {
        if (splitchannelname[i].empty() || splitchannelname[i][0] != '#')
        {
            std::string err = ":" + serverName +
                              " 476 " + client.getNickname() +
                              " " + splitchannelname[i] +
                              " :Bad Channel Mask\r\n";
            send(client.getFd(), err.c_str(), err.size(), 0);
            i++;
            continue;
        }
        if (channels.find(splitchannelname[i]) == channels.end())
            channels[splitchannelname[i]] = Channel(splitchannelname[i]);
        Channel &channel = channels[splitchannelname[i]];
        if (channel.isInviteOnly() && !channel.isInvited(client.getFd()))
        {
            std::string err = ":" + serverName + " 473 " + client.getNickname() + " " + splitchannelname[i] + " :Invite only channel\r\n";
            send(client.getFd(), err.c_str(), err.size(), 0);
            i++;
            continue;
        }
        if (channel.hasPassword())
        {
            std::string key;
            if (i < splitkey.size())
                key = splitkey[i];
            if (key != channel.getPassword())
            {
                std::string err = ":" + serverName + " 475 " +
                                  client.getNickname() + " " + splitchannelname[i] +
                                  " :Cannot join channel (+k)\r\n";
                send(client.getFd(), err.c_str(), err.size(), 0);
                i++;
                continue;
            }
        }
        if (channel.isFull())
        {
            std::string err = ":" + serverName + " 471 " + client.getNickname() + " " + splitchannelname[i] + " :Cannot join channel (+l)\r\n";
            send(client.getFd(), err.c_str(), err.size(), 0);
            i++;
            continue;
        }
        if (channel.isInviteOnly())
            std::cout << "Channel is invite only" << std::endl;
        if (channel.hasMember(client.getFd()))
        {
            i++;
            continue;
        }
        channel.addMember(client.getFd());
        channel.removeInvite(client.getFd());
        if (channel.memberCount() == 1)
            channel.addOperator(client.getFd());
        client.joinChannel(splitchannelname[i]);
        std::string msg = ":" + client.getNickname() + " JOIN " + splitchannelname[i] + "\r\n";
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
            Client &mClient = clients[*it2];

            // std::cout << "names client: " + mClient.getNickname() << std::endl;
            if (channel.isOperator(mClient.getFd()))
                listnames += "@" + mClient.getNickname() + " ";
            else
                listnames += mClient.getNickname() + " ";
            it2++;
        }
        std::string namesMsg = ":" + serverName + " 353 " + client.getNickname() + " = " + splitchannelname[i] + " :" + listnames + "\r\n";
        send(client.getFd(), namesMsg.c_str(), namesMsg.size(), 0);
        std::string endNamesMsg = ":" + serverName + " 366 " + client.getNickname() + " " + splitchannelname[i] + " :End of /NAMES list\r\n";
        send(client.getFd(), endNamesMsg.c_str(), endNamesMsg.size(), 0);
        i++;
    }
}
