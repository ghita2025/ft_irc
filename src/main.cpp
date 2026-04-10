#include "ft_irc.h"

int main(int ac, char *av[])
{
    if (!validateArgs(ac, av))
        return 1;
    int port = atoi(av[1]);
    std::string password(av[2]);
    Server server(port, password);
    server.run();

    return 0;
}