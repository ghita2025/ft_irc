#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
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
#include "Channel.hpp"

#define BACKLOG 10
#define BUFFER_SIZE 512

class Server
{
private:
    int serverFd;
    int port;
    std::string password;
    std::vector<pollfd> fds;
    std::map<int, Client> clients;

public:
    Server(int port, std::string password);
    ~Server();
    void initSocket();
    void run();
    void handleNewClient();
    void handleClientRead(int fd);
    void handleClientWrite(int fd);
    void removeClient(int fd);

    // void processCommand(Client &client, std::string cmd);
};

#endif