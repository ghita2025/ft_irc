#include "Bot.hpp"
#include <sstream>

int main(int ac, char *av[])
{
    if (ac != 4)
    {
        std::cerr << "Usage: ./ircbot <ip> <port> <password>" << std::endl;
        return (1);
    }

    std::string ip       = av[1];
    int         port     = atoi(av[2]);
    std::string password = av[3];

    if (port <= 1024 || port > 65535)
    {
        std::cerr << "Error: invalid port" << std::endl;
        return (1);
    }

    Bot bot(port, ip, password);
    bot.run();

    return (0);
} 