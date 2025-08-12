#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

constexpr int BUF_SIZE = 1024;
constexpr int MAX_EVENTS = 128;
constexpr int THREAD_COUNT = 17;

std::atomic<int> connection_count{0};
epoll_event events[MAX_EVENTS];
sockaddr_in server_addr{AF_INET, htons(8888), 0};

void worker()
{
    std::string str = "Hello,World!";
    char buf[BUF_SIZE];

    for (int i = 0; i < str.size(); ++i) buf[i] = str[i];

    while (true)
    {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1) std::cerr << "socket creating error" << std::endl;

        int bind_res =
            bind(sock, (sockaddr *)&server_addr, sizeof(server_addr));
        if (bind_res == -1) std::cerr << "binding error" << std::endl;

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

    for (int i = 0; i < thread_pool.size(); ++i)
        thread_pool[i] = std::thread(worker);

    for (int i = 0; i < thread_pool.size(); ++i) thread_pool[i].detach();

    auto start = clock();

    while (true)
    {
        auto end = clock();
        if (end - start / (double)CLOCKS_PER_SEC > 1)
            std::cout << connection_count << std::endl;
    }
}