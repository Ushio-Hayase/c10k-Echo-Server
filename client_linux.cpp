#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>

#include <atomic>
#include <cstring>
#include <iostream>

constexpr int MAX_EVENTS = 128;

std::atomic<int> connection_count{0};
epoll_event events[MAX_EVENTS];

int main()
{
    // epoll 인스턴스
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) std::cerr << "epoll creating error" << std::endl;

    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == -1) std::cerr << "socket creating error" << std::endl;

    sockaddr_in server_addr;

    memset(&server_addr, sizeof(server_addr), 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = 8888;
    server_addr.sin_addr.s_addr = 0;

    // 감시할 epoll event 종류
    epoll_event ev;

    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_sock, &ev) == -1)
        std::cerr << "registing epoll event error" << std::endl;

    while (true)
    {
        int wait = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (wait == -1) std::cerr << "waiting epoll error" << std::endl;

        for (int i = 0; i < wait; ++i)
        {
            if (events[i].data.fd == listen_sock)
            {
                socklen_t size = sizeof(sockaddr_in);
                int client_sock =
                    accept(listen_sock, (sockaddr*)&server_addr, &size);

                ev.events = EPOLLIN;
                ev.data.fd = client_sock;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_sock, &ev);
            }
        }
    }
}