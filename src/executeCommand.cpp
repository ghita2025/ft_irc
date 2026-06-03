#include "../include/Channel.hpp"
#include "ft_irc.h"
#include "../include/Command.hpp"

void Server::executeCommand(Client &client, Command &cmd)
{
    std::cout << "name: " << cmd.name << std::endl;
    std::cout << "authenticated: " << client.authenticated << std::endl;
    if (!client.authenticated && cmd.name != "PASS" && cmd.name != "NICK" && cmd.name != "USER")
    {
        std::string err = "451 :You have not registered\r\n";
        send(client.getFd(), err.c_str(), err.size(), 0);
        return ;
    }
    if (cmd.name == "PASS")
        handlePass(client, cmd);
    else if (cmd.name == "NICK")
        handleNick(client, cmd);
    else if (cmd.name == "USER")
        handleUser(client, cmd);
    else if (cmd.name == "JOIN")
        handleJoin(client, cmd);
    else if (cmd.name == "PART")
        handlePart(client, cmd);
    else if (cmd.name == "KICK")
        handleKick(client, cmd);
    else if (cmd.name == "INVITE")
        handleInvite(client, cmd);
    else if (cmd.name == "TOPIC")
        handleTopic(client, cmd);
    else if (cmd.name == "MODE")
        handleMode(client, cmd);
    else if (cmd.name == "PRIVMSG")
        handlePrivmsg(client, cmd);
    else if (cmd.name == "QUIT")
        handleQuit(client, cmd);
}