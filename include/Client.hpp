#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>

class Client
{
    private:
        int fd;
        std::string nickname;
        std::string username;
        bool isRegistered;
        bool hasPassedPassword;

        std::string recvBuffer;
        std::string sendBuffer;

    public:
        Client();
        Client(int fd, std::string name);
        ~Client();

        int &getFd();
        std::string &getUsername();
        std::string &getNickname();
        std::string &getRecvBuffer();
        std::string &getSendBuffer();

        void appendToBuffer(const std::string &data);
        void extractCommands(std::vector<std::string> &commands);
        void queueMessageToSend(const std::string &msg);
};

#endif