#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

void Server::tryAuthenticate(Client& client)
{
    if (client.passOk && client.nickSet && client.userSet)
    {
        client.authenticated = true;
        std::string msg = ":server " + client.getNickname() + " :Welcome to IRC\r\n";
        send(client.getFd(), msg.c_str(), msg.size(), 0);
    }
}