#include "ft_irc.h"

Client::Client() : fd(-1), nickname(""), username(""), isRegistered(false), hasPassedPassword(false) {}

Client::Client(int fd, std::string name) : fd(fd), username(name), isRegistered(false), hasPassedPassword(false) {}

int &Client::getFd()
{
    return fd;
}

std::string &Client::getUsername()
{
    return username;
}

std::string &Client::getNickname()
{
    return nickname;
}
std::string &Client::getRecvBuffer()
{
    return recvBuffer;
}
std::string &Client::getSendBuffer()
{
    return sendBuffer;
}

void Client::appendToBuffer(const std::string &data)
{
    recvBuffer += data;
}

void Client::extractCommands(std::vector<std::string> &commands)
{
    size_t pos;
    while ((pos = recvBuffer.find("\r\n")) != std::string::npos)
    {
        commands.push_back(recvBuffer.substr(0, pos));
        recvBuffer.erase(0, pos + 2);
    }
}
void Client::queueMessageToSend(const std::string &msg)
{
    sendBuffer += msg + "\r\n";
}

Client::~Client() {}
