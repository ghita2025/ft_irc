#ifndef FT_IRC_H
#define FT_IRC_H
#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"
#include <iostream>

int validatePort(std::string portStr);
int validateArgs(int ac, char **av);

#endif