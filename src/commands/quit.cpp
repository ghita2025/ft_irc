#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

void Server::handleQuit(Client& client, Command& cmd)
{
    std::string reason;

    if (cmd.args.size() > 0)
        reason = cmd.args[0];
    else
        reason = "Client Quit";
    std::string msg = ":" + client.getNickname() + " QUIT :" + reason + "\r\n";
    std::set<std::string> userChannels = client.getChannels();
    std::set<std::string>::iterator it = userChannels.begin();
    while (it != userChannels.end())
    {
        std::map<std::string, Channel>::iterator ch = channels.find(*it);
        if (ch == channels.end())
            continue;
        Channel &channel = ch->second;
        std::set<int> members = channel.getMembers();
        std::set<int>::iterator m = members.begin();
        while (m != members.end())
        {
            if (*m != client.getFd())
                send(*m, msg.c_str(), msg.size(), 0);
            m++;
        }
        channel.removeMember(client.getFd());
        if (channel.memberCount() == 0)
            channels.erase(channel.getName());
        it++;
    }
    removeClient(client.getFd());
}