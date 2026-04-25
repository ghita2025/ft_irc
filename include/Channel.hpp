#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>

class Channel
{
    private:
        std::string name;
        std::string topic;
        std::string key;
        bool inviteOnly;
        bool topicRestricted;
        bool hasPass;
        int limit;
        std::string password;
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
        const   std::set<int>& getMembers() const;
        int     memberCount();
        bool    isInviteOnly() const;
        void    setInviteOnly(bool value);
        bool    isTopicRestricted() const;
        void    setTopicRestricted(bool value);
        void    setTopic(const std::string& newTopic);
        const   std::string& getTopic() const;
        bool    hasKey() const;
        void    setKey(const std::string& newKey);
        const   std::string& getKey() const;
        bool    hasPassword() const;
        std::string getPassword() const;
        bool    isFull() const;
        void    setPassword(const std::string& pass);
        void    removePassword();
        void    setLimit(int l);
        void    removeLimit();
};

#endif