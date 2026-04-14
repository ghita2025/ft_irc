#include "Channel.hpp"

Channel::Channel()
{}

Channel::Channel(std::string channelName)
{
    this->name = channelName;
}

void    Channel::addMember(int fd)
{
    this->members.insert(fd);
}

void    Channel::removeMember(int fd)
{
    this->members.erase(fd);
}

bool    Channel::hasMember(int fd)
{
    std::set<int>::iterator i;
    
    i = this->members.begin();
    while (i != this->members.end())
    {
        if (*i == fd)
            return (true);
        i++;
    }
    return (false);
}

void    Channel::addOperator(int fd)
{
    if (this->hasMember(fd))
    {
        this->operators.insert(fd);
    }
}

bool    Channel::isOperator(int fd)
{
    std::set<int>::iterator i;

    i = this->operators.begin();
    while (i != this->operators.end())
    {
        if (*i == fd)
            return (true);
        i++;
    }
    return (false);
}

void    Channel::addInvite(int fd)
{
    this->invited.insert(fd);
}

int     Channel::memberCount()
{
    int size;
    std::set<int>::iterator itmem;
    
    size = 0;
    itmem = this->members.begin();
    while (itmem != this->members.end())
    {
        size++;
        itmem++;
    }
    return (size);
}

bool    Channel::isInvited(int fd)
{
    std::set<int>::iterator i;

    i = this->invited.begin();
    while (i != this->invited.end())
    {
        if (*i == fd)
            return (true);
        i++;
    }
    return (false);
}

void    Channel::removeInvite(int fd)
{
    this->invited.erase(fd);
}

void    Channel::removeOperator(int fd)
{
    this->operators.erase(fd);
}

const std::set<int>& Channel::getMembers() const
{
    return (this->members);
}