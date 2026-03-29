#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <netdb.h>
#include <cstdio>
#include <poll.h>
#include <map>

#define PORT "3490"
#define BACKLOG 10

struct client
{
    int fd;
    std::string recv_buffer;
    std::string send_buffer;
};

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

    // struct sockaddr_storage client_addr;
    // socklen_t addr_size;
    std::map<int, client> clients;

    // Data structure
    std::vector<pollfd> fds;

    // ADD server socket
    pollfd server_fd;
    server_fd.fd = sockfd;
    server_fd.events = POLLIN;
    server_fd.revents = 0;

    fds.push_back(server_fd);

    while (true)
    {
        poll(fds.data(), fds.size(), -1);
        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == sockfd)
                {
                    struct sockaddr_storage client_addr;
                    socklen_t addr_size;

                    int newfd = accept(sockfd, reinterpret_cast<sockaddr *>(&client_addr), &addr_size);
                    if (newfd == -1)
                    {
                        std::cerr << "accept failed\n";
                        continue;
                    }
                    pollfd client_fd;
                    client_fd.fd = newfd;
                    client_fd.events = POLLIN;
                    client_fd.revents = 0;
                    fds.push_back(client_fd);

                    // add ti clients map
                    client c;
                    c.fd = newfd;
                    clients[newfd] = c;
                }
                else
                {
                    char temp_buff[512];
                    std::string msg;
                    int bytes = recv(fds[i].fd, temp_buff, sizeof temp_buff, 0);
                    if(bytes <= 0)
                    {
                        std::cerr << "client disconnected " << std::endl;
                        close(fds[i].fd);
                        clients.erase(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        i--;
                    }
                    else
                    {
                        clients[fds[i].fd].recv_buffer.append(temp_buff);
                        size_t pos;
                        while((pos = clients[fds[i].fd].recv_buffer.find("\r\n")) != std::string::npos)
                        {
                            msg = clients[fds[i].fd].recv_buffer.substr(0,pos);
                            clients[fds[i].fd].recv_buffer = clients[fds[i].fd].recv_buffer.substr(pos + 2);
                            std::cout << "msg : " << msg << std::endl;
                            std::cout << "client buffer" << clients[fds[i].fd].recv_buffer << std::endl;

                            std::string reply = msg + "\r\n";
                            size_t total_sent = 0;
                            clients[fds[i].fd].send_buffer += reply;
                            while(total_sent < clients[fds[i].fd].send_buffer.size())
                            {
                                ssize_t n = send(fds[i].fd, clients[fds[i].fd].send_buffer.c_str() + total_sent, clients[fds[i].fd].send_buffer.size() - total_sent, 0);
                                if (n <= 0)
                                {
                                    std::cerr << "send error or connection closed\n";
                                    close(fds[i].fd);
                                    clients.erase(fds[i].fd);
                                    fds.erase(fds.begin() + i);
                                    i--;
                                    break;
                                }
                                total_sent += n;

                            }
                            clients[fds[i].fd].send_buffer = clients[fds[i].fd].send_buffer.substr(total_sent);
                        }

                    }
                }
            }
        }

        // addr_size = sizeof(struct sockaddr_storage);
        // int newfd = accept(sockfd, reinterpret_cast<sockaddr *>(&client_addr), &addr_size);
        // if (newfd == -1)
        // {
        //     std::cerr << "accept failed\n";
        //     // continue;
        // }
        // std::cout << "New client connected! FD: " << newfd << std::endl;
        // clients.push_back(newfd);
        // std::string buffer;
        // std::string msg;
        // char temp_buffer[20];
        // while (1)
        // {
        //     int bytes = recv(newfd, temp_buffer, sizeof(temp_buffer) - 1, 0);
        //     if (bytes <= 0)
        //     {
        //         std::cout << "client disconnected" << std::endl;
        //         close(newfd);
        //         break;
        //     }
        //     temp_buffer[bytes] = '\0';
        //     buffer.append(temp_buffer);
        //     size_t pos;

        //     while ((pos = buffer.find("\r\n")) != std::string::npos)
        //     {
        //         msg = buffer.substr(0, pos);
        //         buffer = buffer.substr(pos + 2);

        //         std::cout << "\nfinal text = " << msg << std::endl;
        //         std::cout << "buffer text = " << buffer << std::endl;
        //         std::string reply = msg + "\r\n";
        //         size_t total_sent = 0;
        //         size_t msg_size = reply.size();

        //         while (total_sent < msg_size)
        //         {
        //             ssize_t n = send(newfd,
        //                              reply.c_str() + total_sent,
        //                              msg_size - total_sent,
        //                              0);

        //             if (n <= 0)
        //             {
        //                 std::cerr << "send error or connection closed\n";
        //                 close(newfd);
        //                 break;
        //             }

        //             total_sent += n;
        //         }
        //     }
        // }
    }
    close(sockfd);
    return 0;
}
