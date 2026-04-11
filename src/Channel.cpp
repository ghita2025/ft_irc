#include "Channel.hpp"

Channel::Channel()
{}

Channel::Channel(std::string channelName)
{
    name = channelName;
}

bool    Channel::hasMember(int fd)
{
    return (members.find(fd) != members.end());
}

void    Channel::addMember(int fd)
{
    members.insert(fd);
}

void    Channel::removeMember(int fd)
{
    members.erase(fd);
}

std::set<int>::size_type Channel::memberCount() const
{
    return members.size();
}

void    Channel::addOperator(int fd)
{
    operators.insert(fd);
}