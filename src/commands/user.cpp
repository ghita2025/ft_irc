#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

void Server::handleUser(Client& client, Command& cmd)
{
    if (cmd.args.size() < 4)
    {
        std::string err = ":server 461 USER :Not enough parameters\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return ;
    }
    if (client.userSet)
    {
        std::string err = ":server 462 USER :You may not reregister\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return ;
    }
    client.setUsername(cmd.args[0]);
    client.userSet = true;
    tryAuthenticate(client);
}