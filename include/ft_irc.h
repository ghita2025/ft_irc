#ifndef FT_IRC_H
#define FT_IRC_H

#include <map>
#include <poll.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <string>
#include <sstream>
#include <stdlib.h>

#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include <vector>

int validatePort(std::string portStr);
int validateArgs(int ac, char **av);

#endif