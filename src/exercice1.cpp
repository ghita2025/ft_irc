#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <netdb.h>
#include <cstdio>

#define PORT "3490"
#define BACKLOG 10

int main()
{
    struct addrinfo hints, *res, *p;
    int status, yes = 1, sockfd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    status = getaddrinfo(NULL, PORT, &hints, &res);
    if (status == -1)
    {
        std::cout << "getaddrinfo failed :" << gai_strerror(status) << std::endl;
        return 1;
    }
    for (p = res; p != NULL; p = p->ai_next)
    {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1)
        {
            perror("socket");
            continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1)
        {
            perror("setsockopt");
            continue;
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            perror("bind");
            continue;
        }
        break;
    }
    freeaddrinfo(res);
    if (p == NULL)
    {
        std::cerr << "bind failed on all\n";
        return 1;
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        std::cerr << "listen failed\n";
        return 1;
    }
    std::cout << "Server listening on port " << PORT << std::endl;

    struct sockaddr_storage client_addr;
    socklen_t addr_size;
    std::vector<int> clients;

    while (true)
    {
        addr_size = sizeof(struct sockaddr_storage);
        int newfd = accept(sockfd, reinterpret_cast<sockaddr *>(&client_addr), &addr_size);
        if (newfd == -1)
        {
            std::cerr << "accept failed\n";
            // continue;
        }
        std::cout << "New client connected! FD: " << newfd << std::endl;
        clients.push_back(newfd);
        std::string buffer;
        std::string msg;
        char temp_buffer[20];
        while (1)
        {
            int bytes = recv(newfd, temp_buffer, sizeof(temp_buffer) - 1, 0);
            if (bytes <= 0)
            {
                std::cout << "client disconnected" << std::endl;
                close(newfd);
                break;
            }
            temp_buffer[bytes] = '\0';

            std::cout << "Received: " << temp_buffer << std::endl;
            buffer.append(temp_buffer);

            std::cout << "buffer after append : " << buffer << std::endl;
            size_t pos = buffer.find("\r\n");
            while (pos != std::string::npos)
            {

                std::cout << "pos   :   " << pos << "  buffer size   :   " << buffer.size() << std::endl;
                msg = buffer.substr(0, pos);
                buffer = buffer.substr(pos + 2);
                std::cout << std::endl;
                std::cout << "final text = " << msg << std::endl;
                std::cout << std::endl;
                std::cout << "buffer text = " << buffer << std::endl;
                pos = buffer.find("\r\n");
            }
        }
        std::string message("ghita zwena mojtahida kadfiq bkri w kansali flwaqt w ldakhel dyala mzyanghita zwena mojtahida kadfiq bkri w kansali flwaqt w ldakhel dyala mzyanghita zwena mojtahida kadfiq bkri w kansali flwaqt w ldakhel dyala mzyanghita zwena mojtahida kadfiq bkri w kansali flwaqt w ldakhel dyala mzyanghita ldakhel dyala mzyan\n");
        size_t total_sent = 0;
        size_t msg_size = message.size();

        while (total_sent < msg_size)
        {
            ssize_t n = send(newfd,
                             message.c_str() + total_sent,
                             msg_size - total_sent,
                             0);

            if (n <= 0)
            {
                std::cerr << "send error or connection closed\n";
                break;
            }

            total_sent += n;
        }
    }
    close(sockfd);
    return 0;
}