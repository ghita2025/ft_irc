#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>

class Channel
{
    private:
        std::string name;
        std::set<int> members;
        std::set<int> operators;
    public:
        Channel();
        Channel(std::string channelName);
        bool hasMember(int fd);
        void addMember(int fd);
        void removeMember(int fd);
        std::set<int>::size_type memberCount() const;
        void addOperator(int fd);
};

#endif