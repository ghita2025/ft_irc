#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>

class Channel
{
    private:
        std::string name;
        std::string topic;
        std::set<int> members;
        std::set<int> operators;
        std::set<int> invited;
    public:
        Channel();
        Channel(std::string channelName);
        void    addMember(int fd);
        void    removeMember(int fd);
        bool    hasMember(int fd);
        void    addOperator(int fd);
        bool    isOperator(int fd);
        void    addInvite(int fd);
        bool    isInvited(int fd);
        void    removeInvite(int fd);
        void    removeOperator(int fd);
        std::string getName() const;
        const std::set<int>& getMembers() const;
        int     memberCount();
        
};

#endif