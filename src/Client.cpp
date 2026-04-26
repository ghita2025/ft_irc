#include "ft_irc.h"
#include "../include/Channel.hpp"

Client::Client() : fd(-1), nickname(""), username("")
{
    passOk = false;
    nickSet = false;
    userSet = false;
}

Client::Client(int fd, std::string name) : fd(fd), username(name) {}

int &Client::getFd()
{
    return fd;
}

void Client::joinChannel(const std::string& name)
{
    channels.insert(name);
}

void Client::leaveChannel(const std::string& name)
{
    channels.erase(name);
}

std::set<std::string> Client::getChannels() const
{
    return (channels);
}

void Client::setNickname(const std::string &nick)
{
    this->nickname = nick;
}

std::string &Client::getUsername()
{
    return username;
}

void Client::setUsername(const std::string &name)
{
    this->username = name;
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
