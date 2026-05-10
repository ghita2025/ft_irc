#include "ft_irc.h"
#include "Command.hpp"
#include "Parser.hpp"

bool Server::runningServer = true; //added

Server::Server(int port, std::string password) : port(port), password(password)
{
    initSocket();
}
void Server::initSocket()
{
    struct addrinfo hints, *res, *p;
    int yes = 1;
    int status;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_socktype = SOCK_STREAM;

    std::stringstream ss;
    ss << port;
    std::string portStr = ss.str();

    if ((status = getaddrinfo(NULL, portStr.c_str(), &hints, &res)) == -1)
    {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        exit(1);
    }

    for (p = res; p != NULL; p = p->ai_next)
    {
        if ((serverFd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
            continue;

        setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);
        if (bind(serverFd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(serverFd);
            continue;
        }
        break;
    }
    freeaddrinfo(res);
    if (p == NULL)
    {
        std::cerr << "Failed to bind socket" << std::endl;
        exit(1);
    }

    if (listen(serverFd, BACKLOG) == -1)
    {
        std::cerr << "Failed to listen on socket" << std::endl;
        exit(1);
    }
}

Server::~Server()
{
    close(serverFd);
}

void Server::run()
{
    setupSignals();//mine
    pollfd pfd;
    pfd.fd = serverFd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    fds.push_back(pfd);

    while (Server::runningServer)//mine
    {
        if (poll(&fds[0], fds.size(), -1) == -1)
        {
            std::cerr << "Poll error" << std::endl;
            continue;
        }

        for (size_t i = 0; i < fds.size(); i++)
        {
            pollfd &pfd = fds[i];
            if (pfd.revents & (POLLHUP | POLLERR))
            {
                removeClient(pfd.fd);
                continue;
            }
            if (pfd.revents & POLLIN)
            {
                if (pfd.fd == serverFd)
                    handleNewClient();
                else
                    handleClientRead(pfd.fd);
            }
            if (pfd.revents & POLLOUT)
                handleClientWrite(pfd.fd);
        }
    }
}

void Server::handleNewClient()
{
    struct sockaddr_storage clientAddr;
    socklen_t addrSize = sizeof clientAddr;
    int clientFd = accept(serverFd, reinterpret_cast<struct sockaddr *>(&clientAddr), &addrSize);
    if (clientFd == -1)
    {
        std::cerr << "Accept error" << std::endl;
        return;
    }

    fcntl(clientFd, F_SETFL, O_NONBLOCK);
    pollfd pfd;
    pfd.fd = clientFd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    fds.push_back(pfd);
    clients[clientFd] = Client(clientFd, "");
}

void Server::handleClientRead(int fd)
{
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead = recv(fd, buffer, sizeof(buffer), 0);
    if (bytesRead <= 0)
    {
        removeClient(fd);
        return;
    }
    clients[fd].appendToBuffer(std::string(buffer, bytesRead));
    std::vector<std::string> commands;
    clients[fd].extractCommands(commands);
    for (size_t i = 0; i < commands.size(); i++)
    {
        processCommand(clients[fd], commands[i]);                 // parse and execute command
        // Command cmd(commands[i]);
        // executeCommand(clients[fd], cmd);
        std::cout << "Received command from client " << fd << ": " << commands[i] << std::endl;
    }
}

void Server::handleClientWrite(int fd)
{
    Client &client = clients[fd];
    std::string &sendBuffer = client.getSendBuffer();
    ssize_t bytesSent = send(fd, sendBuffer.c_str(), sendBuffer.size(), 0);
    if (bytesSent <= 0)
    {
        removeClient(fd);
        return;
    }
    sendBuffer.erase(0, bytesSent);
    if (sendBuffer.empty())
    {
        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].fd == fd)
            {
                fds[i].events &= ~POLLOUT;
                break;
            }
        }
    }
}

void Server::removeClient(int fd)
{
    close(fd);
    clients.erase(fd);
    for (size_t i = 0; i < fds.size(); i++)
    {
        if (fds[i].fd == fd)
        {
            fds.erase(fds.begin() + i);
            break;
        }
    }
}

void Server::processCommand(Client &client, const std::string &rawString)
{
    std::string command;
    std::vector<std::string> args;

    Parser::parseCommand(rawString, command, args);

    Command cmd;
    cmd.name = command;
    cmd.args = args;

    if (command == "PASS")
        handlePass(client, cmd);
    else if (command == "NICK")
        handleNick(client, cmd);
    else if (command == "USER")
        handleUser(client, cmd);
    else if (command == "JOIN")
        handleJoin(client, cmd);
    else if (command == "PRIVMSG")
        handlePrivmsg(client, cmd);
}

