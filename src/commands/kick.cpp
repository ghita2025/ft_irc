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
            return ;
        Client *target = getClientByNick(targetNick);
        if (!target)
            return;
        if (!cl.hasMember(target->getFd()))
            return ;
        cl.removeMember(target->getFd());
        cl.removeOperator(target->getFd());
        std::cout << "dakchi howa hadak kick" << std::endl;
    }
}