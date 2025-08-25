#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <cstring>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

constexpr int BUF_SIZE = 1024;
constexpr int MAX_EVENTS = 1024;
constexpr int THREAD_COUNT = 16;
constexpr int MAX_CONNECT = 12000;

std::atomic<int> connection_count{0};
int epoll_fd;

sockaddr_in server_addr;

void worker()
{
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        std::cerr << "worker epoll_create1 failed: " << strerror(errno)
                  << std::endl;
        return;
    }

    std::string str = "Hello,World!";
    char send_buf[BUF_SIZE];
    std::fill(send_buf, send_buf + BUF_SIZE, 0);
    memcpy(send_buf, str.c_str(), str.length());

    for (int i = 0; i < MAX_CONNECT / THREAD_COUNT; ++i)
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1)
        {
            std::cerr << "socket creating error: " << strerror(errno)
                      << std::endl;
            continue;
        }
        if (connect(sock, (sockaddr*)&server_addr, sizeof(sockaddr)) == -1)
        {
            std::cerr << "connecting error: " << strerror(errno) << std::endl;
            close(sock);
            continue;
        }
        if (send(sock, send_buf, BUF_SIZE, 0) == -1)
        {
            std::cerr << "sending error: " << strerror(errno) << std::endl;
            close(sock);
            continue;
        }

        epoll_event ev;
        ev.data.fd = sock;
        ev.events = EPOLLIN;
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &ev);
        connection_count++;
    }

    epoll_event events[MAX_EVENTS];
    while (true)
    {
        int ev_cnt = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < ev_cnt; ++i)
        {
            if (events[i].events & EPOLLIN)
            {
                int sock = events[i].data.fd;
                char recv_buf[BUF_SIZE];
                int recv_res = recv(sock, recv_buf, BUF_SIZE, 0);

                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sock, nullptr);
                close(sock);
                --connection_count;

                int new_sock = socket(AF_INET, SOCK_STREAM, 0);
                if (new_sock == -1) continue;
                if (connect(new_sock, (sockaddr*)&server_addr,
                            sizeof(sockaddr)) == -1)
                {
                    close(new_sock);
                    continue;
                }
                if (send(new_sock, send_buf, BUF_SIZE, 0) == -1)
                {
                    close(new_sock);
                    continue;
                }
                epoll_event ev;
                ev.data.fd = new_sock;
                ev.events = EPOLLIN;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, new_sock, &ev);
                ++connection_count;
            }
        }
    }
}

int main()
{
    std::vector<std::thread> thread_pool;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0)
    {
        std::cerr << "\nInvalid address/ Address not supported \n";
    }

    epoll_fd = epoll_create1(0);

    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        thread_pool.emplace_back(worker);
    }

    auto start = std::chrono::system_clock::now();

    while (true)
    {
        auto end = std::chrono::system_clock::now();
        if ((end - start) > std::chrono::seconds(1))
        {
            std::cerr << connection_count << std::endl;
            start = end;
        }
    }
}