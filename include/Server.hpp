#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <map>
#include <string>
#include <poll.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <string>
#include <limits>
#include <cstdio>
#include <iomanip>
#include <cmath>
#include <stdint.h>
#include <sys/socket.h>


#define BACKLOG 10
#define BUFFER_SIZE 512

class Client;
class Command;
class Channel;


class Server
{
    private:
        int serverFd;
        int port;
        std::string password;
        std::vector<pollfd> fds;
        std::map<int, Client> clients;
        std::map<std::string, Channel> channels;
    public:
        Server(int port, std::string password);
        ~Server();
        void initSocket();
        void run();
        void handleNewClient();
        void handleClientRead(int fd);
        void handleClientWrite(int fd);
        void removeClient(int fd);
        void handleJoin(Client& client, Command& cmd);
        void handleKick(Client& client, Command& cmd);
        void handlePrivmsg(Client& client, Command& cmd);
        void handlePart(Client& client, Command& cmd);
        Client* getClientByNick(const std::string &nick);

    // void processCommand(Client &client, std::string cmd);
};

#endif