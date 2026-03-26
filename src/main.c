#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT "3490"
#define BACKLOG 10

int main()
{
    struct addrinfo hints{}, *res, *p;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    int sockfd, new_fd;
    int yes = 1;

    // 1. prepare hints
    hints.ai_family = AF_UNSPEC;      // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;  // TCP
    hints.ai_flags = AI_PASSIVE;      // bind to all interfaces

    // 2. getaddrinfo
    int status = getaddrinfo(NULL, PORT, &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
        return 1;
    }

    // 3. loop through results
    for (p = res; p != nullptr; p = p->ai_next)
    {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd == -1)
            continue;

        // 🔥 important
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
            perror("setsockopt");
            return 1;
        }

        if (bind(sockfd,
                 reinterpret_cast<struct sockaddr *>(p->ai_addr),
                 p->ai_addrlen) == -1)
        {
            close(sockfd);
            continue;
        }

        break;
    }

    if (p == nullptr) {
        std::cerr << "Failed to bind\n";
        return 1;
    }

    freeaddrinfo(res);

    // 4. listen
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    // 5. accept one client (for now)
    addr_size = sizeof their_addr;
    new_fd = accept(sockfd,
        reinterpret_cast<struct sockaddr *>(&their_addr),
        &addr_size);

    if (new_fd == -1) {
        perror("accept");
        return 1;
    }

    std::cout << "Client connected! fd = " << new_fd << std::endl;

    // test close
    close(new_fd);
    close(sockfd);

    return 0;
}