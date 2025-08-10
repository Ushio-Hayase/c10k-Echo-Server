#include <WinSock2.h>

#include <chrono>
#include <ctime>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

constexpr int MAX_BUFSIZE = 1024;

int connection = 0;

int main()
{
    std::ios_base::sync_with_stdio(0);
    std::cout.tie(0);

    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) return 1;

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) return 1;

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (SOCKADDR*)&server_addr, sizeof(server_addr)) ==
        SOCKET_ERROR)
    {
        std::cout << "Connect failed." << std::endl;
        return 1;
    }

    std::cout << "Connected to server." << std::endl;

    char recv_buffer[MAX_BUFSIZE];
    char send_buffer[MAX_BUFSIZE];

    std::fill_n(send_buffer, MAX_BUFSIZE, 'x');

    std::chrono::system_clock::time_point start =
        std::chrono::system_clock::now();

    while (true)
    {
        if (send(client_socket, send_buffer, MAX_BUFSIZE, 0) == SOCKET_ERROR)
        {
            break;
        }

        recv(client_socket, recv_buffer, MAX_BUFSIZE, 0);
        ++connection;
        clock_t now = clock();
        std::chrono::duration<double> duration =
            std::chrono::system_clock::now() - start;
        if (duration.count() >= 1)
        {
            std::cerr << connection / duration.count() << " Message per second"
                      << std::endl;
            start = std::chrono::system_clock::now();
            connection = 0;
        }
    }

    closesocket(client_socket);
    WSACleanup();

    return 0;
}