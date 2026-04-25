#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

void Server::handleMode(Client& client, Command& cmd)
{
    if (cmd.args.size() < 2)
        return ;
    std::string channelName = cmd.args[0];
    if (channels.find(channelName) == channels.end())
        return ;
    Channel &channel = channels[channelName];
    std::string modes = cmd.args[1];
    bool add = true;
    size_t paramindex = 2;
    size_t i = 0;
    while (i < modes.size())
    {
        char c = modes[i];
        if (c == '+')
        {
            add = true;
            i++;
            continue ;
        }
        if (c == '-')
        {
            add = false;
            i++;
            continue ;
        }
        if (c == 'i')
            channel.setInviteOnly(add);
        else if (c == 'k')
        {
            if (add)
                if (paramindex < cmd.args.size())
                    channel.setPassword(cmd.args[paramindex++]);
            else
                channel.removePassword();
        }
        else if (c == 'l')
        {
            if (add)
                if (paramindex < cmd.args.size())
                    channel.setLimit(std::atoi(cmd.args[paramindex++].c_str()));
            else
                channel.removeLimit();
        }
        else if (c == 'o')
        {
            if (paramindex < cmd.args.size())
            {
                Client* target = getClientByNick(cmd.args[paramindex++]);
                if (target)
                {
                    int fd = target->getFd();
                    if (add)
                        channel.addOperator(fd);
                    else
                        channel.removeOperator(fd);
                }
            }
        }
        i++;
    }
    std::string msg = ":" + client.getNickname() + " MODE " + channelName + " " + modes + "\r\n";
    std::set<int> members = channel.getMembers();
    std::set<int>::iterator it = members.begin();
    while (it != members.end())
    {
        send(*it, msg.c_str(), msg.size(), 0);
        it++;
    }
    std::cout << "MODE applied: " << modes << " on " << channelName << std::endl;
}