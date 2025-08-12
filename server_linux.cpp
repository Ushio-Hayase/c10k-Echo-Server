#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <thread>
#include <vector>

constexpr int EVENT_SIZE = 1024;
constexpr int BUF_SIZE = 1024;
constexpr int THREAD_COUNT = 17;

sockaddr_in server_addr{AF_INET, 8888, 0};
socklen_t sock_len = sizeof(sockaddr);

void worker(int epoll_fd, int listen_sock)
{
    while (true)
    {
        epoll_event ev[EVENT_SIZE];
        int ev_cnt = epoll_wait(epoll_fd, ev, EVENT_SIZE, -1);

        for (int i = 0; i < ev_cnt; ++i)
        {
            if (ev[i].data.fd == listen_sock)
            {
                int acpt_sock =
                    accept(listen_sock, (sockaddr*)&server_addr, &sock_len);

                char buf[BUF_SIZE];
                int recv_bytes = recv(acpt_sock, buf, BUF_SIZE, 0);
                if (recv_bytes == -1)
                    std::cerr << "data recving error" << std::endl;

                int send_bytes = send(acpt_sock, buf, BUF_SIZE, 0);
                if (send_bytes == -1)
                    std::cerr << "data sending error" << std::endl;
            }
        }
    }
}

int main()
{
    std::vector<std::thread> thread_pool(THREAD_COUNT);

    int epoll_fd = epoll_create1(0);

    int listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == -1) std::cerr << "socket craeting error" << std::endl;

    int bind_res = bind(listen_sock, (sockaddr*)&server_addr, sock_len);
    if (bind_res == -1) std::cerr << "binding error" << std::endl;

    int listen_res = listen(listen_sock, EVENT_SIZE);
    if (listen_res == -1) std::cerr << "listening error" << std::endl;

    epoll_event ev;
    ev.data.fd = listen_sock;
    ev.events = EPOLLIN;

    int ep_res = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_sock, &ev);
    if (ep_res == -1) std::cerr << "epoll registing error" << std::endl;

    for (int i = 0; i < thread_pool.size(); ++i)
        thread_pool[i] = std::thread(worker, epoll_fd, listen_sock);

    for (int i = 0; i < thread_pool.size(); ++i) thread_pool[i].join();
}