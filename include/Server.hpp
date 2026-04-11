#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <map>
#include <string>
#include <poll.h>

#define BACKLOG 10
#define BUFFER_SIZE 512

class Client;

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