#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

constexpr int EVENT_SIZE = 1024;
constexpr int BUF_SIZE = 1024;
constexpr int THREAD_COUNT = 17;

sockaddr_in server_addr;
socklen_t sock_len = sizeof(sockaddr);

void worker(int epoll_fd, int listen_sock)
{
    while (true)
    {
        epoll_event ev[EVENT_SIZE];
        int ev_cnt = epoll_wait(epoll_fd, ev, EVENT_SIZE, -1);

        for (int i = 0; i < ev_cnt; ++i)
        {
            if (ev[i].data.fd == listen_sock && ev[i].events & EPOLLIN)
            {
                sockaddr_in client_addr;

                int acpt_sock =
                    accept(listen_sock, (sockaddr*)&client_addr, &sock_len);

                epoll_event cur_ev;
                cur_ev.data.fd = acpt_sock;
                cur_ev.events = EPOLLIN;

                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, acpt_sock, &cur_ev);
            }
            else if (ev[i].events & EPOLLIN)
            {
                int acpt_sock = ev[i].data.fd;
                char buf[BUF_SIZE];
                int recv_bytes = recv(acpt_sock, buf, BUF_SIZE, 0);
                if (recv_bytes > 0)
                {
                    int send_bytes = send(acpt_sock, buf, BUF_SIZE, 0);
                    if (send_bytes == -1)
                        std::cerr << "data sending error" << std::endl;
                }
                else if (recv_bytes == -1)
                    std::cerr << "data recving error" << std::endl;

                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, acpt_sock, nullptr);
            }
        }
    }
}

int main()
{
    std::vector<std::thread> thread_pool;

    server_addr.sin_port = htons(8888);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = 0;
    memset(&(server_addr.sin_zero), 0, 8);

    int epoll_fd = epoll_create1(0);

    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == -1) std::cerr << "socket craeting error" << std::endl;
    std::cout << "socket creating" << std::endl;

    int bind_res = bind(listen_sock, (sockaddr*)&server_addr, sock_len);
    if (bind_res == -1) std::cerr << "binding error" << std::endl;
    std::cout << "socket binding" << std::endl;

    int listen_res = listen(listen_sock, EVENT_SIZE);
    if (listen_res == -1) std::cerr << "listening error" << std::endl;
    std::cout << "socket listening start" << std::endl;

    epoll_event listen_ev;
    listen_ev.data.fd = listen_sock;
    listen_ev.events = EPOLLIN;

    int ep_res = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_sock, &listen_ev);
    if (ep_res == -1) std::cerr << "epoll registing error" << std::endl;

    for (int i = 0; i < THREAD_COUNT; ++i)
        thread_pool.emplace_back(worker, epoll_fd, listen_sock);

    for (int i = 0; i < THREAD_COUNT; ++i) thread_pool[i].join();
}