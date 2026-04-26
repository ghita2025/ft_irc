#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

bool Server::nicknameExists(const std::string &nick)
{
    std::map<int, Client>::iterator it = clients.begin();
    while (it != clients.end())
    {
        if (it->second.getNickname() == nick)
            return (true);
        it++;
    }
    return (false);
}

void Server::handleNick(Client& client, Command& cmd)
{
    if (cmd.args.size() < 1)
    {
        std::string err = "461 NICK :Not enough parameters\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return ;
    }
    std::string nick = cmd.args[0];
    if (nicknameExists(nick))
    {
        std::string err = "433 :Nickname already in use\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return ;
    }
    client.setNickname(nick);
    client.nickSet = true;
}