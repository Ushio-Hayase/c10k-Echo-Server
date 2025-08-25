#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <chrono>
#include <cstring>
#include <iostream>
#include <queue>
#include <thread>
#include <vector>

constexpr int BUF_SIZE = 1024;
constexpr int MAX_EVENTS = 128;
constexpr int THREAD_COUNT = 17;
constexpr int MAX_CONNECT = 12000;

std::atomic<int> connection_count{0};
std::atomic<int> event_counter{0};
int ev_cnt, epoll_fd;
epoll_event events[MAX_EVENTS];
sockaddr_in server_addr;

void worker()
{
    std::string str = "Hello,World!";
    char buf[BUF_SIZE];
    std::fill(buf, buf + BUF_SIZE, '\0');

    for (int i = 0; i < str.size(); ++i) buf[i] = str[i];

    while (true)
    {
        for (; event_counter < ev_cnt; ++event_counter)
        {
            if (events[event_counter].events & EPOLLIN)
            {
                char recv_buf[BUF_SIZE];

                int sock = events[event_counter].data.fd;

                int recv_res = recv(sock, recv_buf, BUF_SIZE, 0);
                if (recv_res == -1) std::cerr << "recving error" << std::endl;

                close(sock);

                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, sock, nullptr);

                --connection_count;
            }
        }

        if (connection_count < MAX_CONNECT)
        {
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock == -1)
            {
                std::cerr << "socket creating error" << std::endl;
                continue;
            }

            ++connection_count;

            int con_res =
                connect(sock, (sockaddr*)&server_addr, sizeof(sockaddr));
            if (con_res == -1)
            {
                std::cerr << "connecting error" << std::endl;
                close(sock);
                continue;
            }

            int res = send(sock, buf, BUF_SIZE, 0);
            if (res == -1)
            {
                std::cerr << "sending error" << std::endl;
                close(sock);
                continue;
            }

            epoll_event ev;
            ev.data.fd = sock;
            ev.events = EPOLLIN;

            epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock, &ev);
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

    for (int i = 0; i < THREAD_COUNT; ++i) thread_pool.emplace_back(worker);

    auto start = std::chrono::system_clock::now();

    while (true)
    {
        auto end = std::chrono::system_clock::now();
        if ((end - start) > std::chrono::seconds(1))
        {
            std::cerr << connection_count << std::endl;
            start = end;
        }
        if (event_counter > MAX_EVENTS)
        {
            ev_cnt = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
            event_counter = 0;
        }
    }
}