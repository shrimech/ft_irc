#include "server.hpp"


int main(void)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "failed to create socket" << std::endl;
        return 1;
    }
    else
        std::cout << "socket fd : " << sockfd << std::endl;

    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        std::cerr << "failed to set socket option : " << strerror(errno) << std::endl;
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_port = htons(6766);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int bnd = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (bnd < 0)
    {
        std::cerr << "failed to bind socket" << std::endl;
        return 1;
    }
    else
        std::cout << "socket is binded" << std::endl;

    
    int lsn = listen(sockfd, SOMAXCONN);
    if (lsn < 0)
    {
        std::cerr << "failed to listen" << std::endl;
        return 1;
    }
    else
        std::cout << "socket is listening on socket : " << sockfd << std::endl;

    // std::vector<int> cltSock;
    std::vector<struct pollfd> plfd;
    std::map<int, std::string> clt_buff;
    struct pollfd srv;
    srv.fd = sockfd;
    srv.events = POLLIN;
    srv.revents = 0;
    plfd.push_back(srv);
    while (true)
    {
        int ready = poll(&plfd[0], plfd.size(), -1);
        if (ready < 0) 
        {
            std::cerr << "poll failed" << std::endl;
            break;
        }
        for (size_t i = 0; i < plfd.size(); i++)
        {
            if (plfd[i].revents == 0)
                continue;
            if (plfd[i].fd == sockfd)
            {
                int n = accept(sockfd, NULL, 0); //-1 wach error wla NON blocking
                if (n == -1)
                {
                    if (errno != EAGAIN && errno != EWOULDBLOCK)
                        std::cerr << "failed to accept" << std::endl;
                }
                else
                {
                    std::cout << "socket client fd : " << n << std::endl;
                    int flags = fcntl(n, F_GETFL, 0);
                    if (flags != -1)
                        fcntl(n, F_SETFL, flags | O_NONBLOCK);

                    struct pollfd clt;
                    clt.fd = n;
                    clt.events = POLLIN;
                    clt.revents = 0;
                    plfd.push_back(clt);
                }
            }
            else 
            {
                int curr_fd = plfd[i].fd;
                if (plfd[i].revents & POLLIN)
                {
                    std::vector<char> s(1024,'\0');
                    ssize_t r = recv(curr_fd, &s[0], s.size() - 1, 0);
                    if (r > 0)
                    {
                        clt_buff[curr_fd].append(&s[0], r);
                        size_t pos;
                        while ((pos = clt_buff[curr_fd].find("\n")) != std::string::npos) 
                        {    
                            std::string comd = clt_buff[curr_fd].substr(0, pos);
                            clt_buff[curr_fd].erase(0, pos + 2);
                            if (!comd.empty() && comd[comd.length() - 1] == '\r')
                                comd.erase(comd.length() - 1);

                            std::cout << "Full command received from " << curr_fd << " : " << comd << std::endl;
                        }
                        // s.push_back('\0');
                        // std::cout << "S : " << &s[0] << "R : " << r << std::endl;
                    }
                    else if (r == 0)
                    {
                        std::cout << "client " << curr_fd << " disconnected" << std::endl;
                        clt_buff.erase(curr_fd);
                        close(curr_fd);
                        plfd.erase(plfd.begin() + i);
                        i--;
                    }
                    else if (errno != EAGAIN && errno != EWOULDBLOCK)
                            std::cerr << "failed to receive" << std::endl;
                }
                if (plfd[i].revents & (POLLERR | POLLHUP | POLLNVAL))
                {
                    std::cerr << "connection lost on fd " << curr_fd << std::endl;
                    clt_buff.erase(curr_fd);
                    close(curr_fd);
                    plfd.erase(plfd.begin() + i);
                    i--;
                }
            }
        }
    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //     int n = accept(sockfd, NULL, 0); //-1 wach error wla NON blocking
    //     if (n == -1)
    //     {
    //         if (errno == EAGAIN || errno == EWOULDBLOCK)
    //         {

    //         }
    //         else
    //             std::cerr << "failed to accept" << std::endl;
    //     }
    //     else
    //     {
    //         int flags = fcntl(fd, F_GETFL, 0);
    //         if (flags != -1)
    //             fcntl(fd, F_SETFL, flags | O_NONBLOCK);

    //         std::vector<char> s(1024,'\0');
    //         // s.push_back("");
    //         if (!s.empty())
    //         {
    //             ssize_t r = recv(n, &s[0], s.size() - 1, 0);
    //             if (r > 0)
    //             {
    //                 clt_buff[n].append(&s[0], r);
    //                 size_t pos;
    //                 while ((pos = clt_buff[n].find("\r\n")) != std::string::npos) 
    //                 {    
    //                     std::string comd = clt_buff[n].substr(0, pos);
    //                     clt_buff[n].erase(0, pos + 2);
    //                     std::cout << "Full command received from " << n << " : " << comd << std::endl;
    //                 }
    //                 // s.push_back('\0');
    //                 // std::cout << "S : " << &s[0] << "R : " << r << std::endl;
    //             }
    //             else if (r == 0)
    //             {
    //                 clt_buff.erase(n);
    //                 close(n);
    //             }
    //             else
    //             {
    //                 if (errno == EAGAIN || errno == EWOULDBLOCK)
    //                 {

    //                 }
    //                 else
    //                     std::cerr << "failed to receive" << std::endl;
    //             }
    //         }
    //     }
    //     // if (n < 0)
    //     //     std::cout << "failed to accept" << std::endl;
    //     // else
    //     //     std::cout << "socket client fd : " << n << std::endl;

    //     cltSock.push_back(n);
//////////////////////////////////////////////////////////////////////////////////////////////////////
    for (size_t i = 0;i < plfd.size();i++)
    {
        close(plfd[i].fd);
    }
    // close(sockfd);
    return 0;
}

//cat /proc/sys/net/core/somaxconn