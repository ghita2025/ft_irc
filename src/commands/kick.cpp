#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

Client* Server::getClientByNick(const std::string &nick)
{
    std::map<int, Client>::iterator it;
    
    it = this->clients.begin();
    while (it != this->clients.end())
    {
        if (it->second.getNickname() == nick)
            return (&it->second);
        ++it;
    }
    return (NULL);
}

void    Server::handleKick(Client& client, Command& cmd)
{
    if (cmd.args.size() < 2)
        return ;
    std::string channelName = cmd.args[0];
    std::string targetNick = cmd.args[1];
    if (this->channels.find(channelName) != this->channels.end())
    {
        Channel &cl = this->channels[channelName];
        if (cl.isOperator(client.getFd()) == false)
        {
            std::string err = ":" + client.getNickname() + " 482 " + channelName + " :You're not channel operator\r\n";
            send(client.getFd(), err.c_str(), err.size(), 0);
            return ;
        }
        Client *target = getClientByNick(targetNick);
        if (!target)
        {
            std::string err = "401 No such nick\r\n";
            send(client.getFd(), err.c_str(), err.size(), 0);
            return ;
        }
        if (!cl.hasMember(target->getFd()))
        {
            std::string err = "441 They aren't on that channel\r\n";
            send(client.getFd(), err.c_str(), err.size(), 0);
            return ;
        }
        std::string msg = ":" + client.getNickname() + " KICK " + channelName + " " + targetNick + " :kicked\r\n";
        std::set<int> members = cl.getMembers();
        std::set<int>::iterator it = members.begin();
        while (it != members.end())
        {
            send(*it, msg.c_str(), msg.size(), 0);
            it++;
        }
        cl.removeMember(target->getFd());
        cl.removeOperator(target->getFd());
        //debug
        std::cout << "dakchi howa hadak kick" << std::endl;
    }
    else
    {
        std::string err = "403 No such channel\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return ;
    }
}