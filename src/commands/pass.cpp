#include "../../include/Server.hpp"
#include "../../include/Command.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

void    Server::handlePass(Client& client, Command& cmd)
{
    if (cmd.args.size() < 1)
    {
        std::string err = "461 PASS :Not enough parameters\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return ;
    }
    if (client.passOk)
    {
        std::string err = "462 :You may not reregister\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return ;
    }
    if (cmd.args[0] != this->password)
    {
        std::string err = "464 :Password incorrect\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return ;
    }
    client.passOk = true;
}