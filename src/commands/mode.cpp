#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

static bool isModeToken(const std::string &token)
{
    return !token.empty() && (token[0] == '+' || token[0] == '-');
}

static bool isChannelToken(const std::string &token)
{
    return !token.empty() && token[0] == '#';
}

void Server::handleMode(Client &client, Command &cmd)
{
    if (cmd.args.size() < 2) 
        return;

    std::string channelName;
    std::string modes;
    std::vector<std::string> modeParams;

    std::cout << "arg 0: " << cmd.args[0] << std::endl; // debug
    std::cout << "arg 1: " << cmd.args[1] << std::endl; // debug
    std::cout << "arg 2: " << cmd.args[2] << std::endl; // debug
    std::cout << "arg 3: " << cmd.args[3] << std::endl; // debug
    if (cmd.args.size() >= 2 && isChannelToken(cmd.args[0]) && isModeToken(cmd.args[1]))
    {
        channelName = cmd.args[0];
        modes = cmd.args[1];
        modeParams.assign(cmd.args.begin() + 2, cmd.args.end());
    }
    else if (cmd.args.size() >= 3 && isModeToken(cmd.args[1]) && isChannelToken(cmd.args[2]))
    {
        channelName = cmd.args[2];
        modes = cmd.args[1];
        modeParams.push_back(cmd.args[0]);
        modeParams.insert(modeParams.end(), cmd.args.begin() + 3, cmd.args.end());
    }
    else if (cmd.args.size() >= 3 && isChannelToken(cmd.args[1]) && isModeToken(cmd.args[2]))
    {
        channelName = cmd.args[1];
        modes = cmd.args[2];
        modeParams.push_back(cmd.args[0]);
        modeParams.insert(modeParams.end(), cmd.args.begin() + 3, cmd.args.end());
    }
    else
    {
        std::string err = "MODE: " + client.getNickname() + " :Invalid MODE syntax\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return;
    }
    // debug
    std::vector<std::string>::iterator itt = modeParams.begin();
    std::cout << "Mode parameters: ";
    while (itt != modeParams.end())
    {
        std::cout << *itt << "|" << std::endl;
        itt++;
    }
    // debug
    if (channels.find(channelName) == channels.end())
    {
        std::string err = "MODE:" + client.getNickname() + " 403 " + channelName + " :No such channel\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return;
    }
    Channel &channel = channels[channelName];
    if (!channel.isOperator(client.getFd()))
    {
        std::string err = "MODE:" + client.getNickname() + " 482 " + channelName + " :You're not channel operator\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return;
    }
    if (!isModeToken(modes) || modes.empty())
    {
        std::string err = "MODE: " + channelName + " :Invalid mode\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return;
    }
    std::cout << "Applying modes: " << modes << std::endl; // debug
    bool add = true;
    size_t paramindex = 0;
    size_t i = 0;
    while (i < modes.size())
    {
        char c = modes[i];
        std::cout << "Processing mode char: " << c << std::endl; // debug
        if (c == '+')
        {
            add = true;
            i++;
            continue;
        }
        if (c == '-')
        {
            add = false;
            i++;
            continue;
        }
        if (c == 'i')
            channel.setInviteOnly(add);
        else if (c == 'k')
        {
            if (add)
            {
                if (paramindex < modeParams.size())
                    channel.setPassword(modeParams[paramindex++]);
                else
                    channel.removePassword();
            }
        }
        else if (c == 'l')
        {
            if (add)
            {
                if (paramindex < modeParams.size())
                    channel.setLimit(std::atoi(modeParams[paramindex++].c_str()));
                else
                    channel.removeLimit();
            }
        }
        else if (c == 'o')
        {
            if (paramindex < modeParams.size())
            {
                Client *target = getClientByNick(modeParams[paramindex++]);
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
    // debug
    std::cout << "MODE applied: " << modes << " on " << channelName << std::endl;
}