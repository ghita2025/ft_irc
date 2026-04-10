#include "ft_irc.h"

int validatePort(std::string portStr)
{
    int port;
    char *endptr = NULL;

    port = strtod(portStr.c_str(), &endptr);
    if (*endptr != '\0' || port <= 1024 || port > 65535)
        return 0;
    return 1;
}

int validateArgs(int ac, char **av)
{
    if (ac < 3)
    {
        std::cerr << "Error : <usage>  ./ircserv <port> <password>" << std::endl;
        return 0;
    }

    if (!validatePort(av[1]))
        return 0;
    return 1;
}