#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <cstring>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

constexpr int BUF_SIZE = 1024;
constexpr int MAX_EVENTS = 128;
constexpr int THREAD_COUNT = 17;

std::atomic<int> connection_count{0};
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
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) std::cerr << "socket creating error" << std::endl;

        int con_res = connect(sock, (sockaddr *)&server_addr, sizeof(sockaddr));
        if (con_res == -1) std::cerr << "connecting error" << std::endl;

        ++connection_count;

        int res = send(sock, buf, BUF_SIZE, 0);
        if (res == -1) std::cerr << "sending error" << std::endl;

        char recv_buf[BUF_SIZE];

        int recv_res = recv(sock, recv_buf, BUF_SIZE, 0);
        if (recv_res == -1) std::cerr << "recving error" << std::endl;

        close(sock);

        --connection_count;
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

    for (int i = 0; i < THREAD_COUNT; ++i) thread_pool.emplace_back(worker);

    clock_t start = clock();

    while (true)
    {
        clock_t end = clock();
        if ((end - start) / (double)CLOCKS_PER_SEC > 1)
        {
            std::cerr << connection_count << std::endl;
            start = end;
        }
    }
}