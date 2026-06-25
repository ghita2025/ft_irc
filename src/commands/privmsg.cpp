#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

void    Server::handlePrivmsg(Client& client, Command& cmd)
{
    if (cmd.args.size() < 2)
    {
        std::string err = ":server 461 PRIVMSG :Not enough parameters\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return ;
    }
        std::string target = cmd.args[0];
        std::string message = cmd.args[1];
        std::cout << target << message << std::endl;
    std::string fullMsg = ":" + client.getNickname() + " PRIVMSG " + target + " :" + message + "\r\n";
    
    if (target[0] == '#')
    {
        std::map<std::string, Channel>::iterator it;
        
        it = channels.find(target);
        if (it == channels.end())
        {
            std::string err = ":server 403 " + target + " :No such channel\r\n";
            send(client.getFd(), err.c_str(), err.size(), 0);
            return ;
        }
        Channel &channel = it->second;
        std::set<int> members = channel.getMembers();
        std::set<int>::iterator m = members.begin();
        while (m != members.end())
        {
            if (*m != client.getFd())
                send(*m, fullMsg.c_str(), fullMsg.size(), 0);
            m++;
        }
    }
    else
    {
        Client *targetClient;
        
        targetClient = getClientByNick(target);
        if (!targetClient)
        {
            std::string err = ":server 401 " + target + " :No such nick\r\n";
            send(client.getFd(), err.c_str(), err.size(), 0);
            return ;
        }
        send(targetClient->getFd(), fullMsg.c_str(), fullMsg.size(), 0);
    }
}