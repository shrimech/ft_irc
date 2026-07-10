#include "server.hpp"


int main(void)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        std::cout << "failed to create socket" << std::endl;
    else
        std::cout << "socket fd : " << sockfd << std::endl;

    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    // setsockopt(sockfd, IPPROTO_TCP, SO_REUSEADDR, );

    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_port = htons(6766);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int bnd = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (bnd < 0)
        std::cout << "failed to bind socket" << std::endl;
    else
        std::cout << "socket is binded" << std::endl;

    
    int lsn = listen(sockfd, SOMAXCONN);
    if (lsn < 0)
        std::cout << "failed to listen" << std::endl;
    else
        std::cout << "socket is listening on socket : " << sockfd << std::endl;

    std::vector<int> cltSock;
    std::vector<struct pollfd> plfd;
    while (true)
    {
        int n = accept(sockfd, NULL, 0); //-1 wach error wla NON blocking
        fcntl(n, F_SETFL, O_NONBLOCK);
        char s[4096];
        ssize_t r = recv(n, s, 40, 0);
        s[40] = '\0';
        std::cout << "S : " << s << "R : " << r << std::endl;
        // if (n < 0)
        //     std::cout << "failed to accept" << std::endl;
        // else
        //     std::cout << "socket client fd : " << n << std::endl;

        cltSock.push_back(n);
    }

    for (size_t i = 0;i < cltSock.size();i++)
    {
        close(cltSock[i]);
    }
    close(sockfd);
    return 0;
}

//cat /proc/sys/net/core/somaxconn