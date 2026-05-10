#include "ft_irc.h"
#include "Server.hpp"

void Server::signalHandler(int signum)
{
    (void)signum;
    Server::runningServer = false;
}

void Server::setupSignals()
{
    struct sigaction sa;
    struct sigaction sa_pipe;

    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGINT);
    sigaddset(&sa.sa_mask, SIGQUIT);
    sa.sa_flags   = 0;
    sa.sa_handler = Server::signalHandler;

    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);

    sigemptyset(&sa_pipe.sa_mask);
    sa_pipe.sa_flags   = 0;
    sa_pipe.sa_handler = SIG_IGN;

    sigaction(SIGPIPE, &sa_pipe, NULL);
}

