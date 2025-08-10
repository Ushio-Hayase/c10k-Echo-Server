#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

#include <iostream>
#include <thread>
#include <vector>

constexpr int MAX_BUFSIZE = 1024;
constexpr int MAX_WORKERTHREAD = 8;
constexpr int MAX_THREAD = 17;

struct ClientContext
{
    SOCKET socket_client_;
    WSAOVERLAPPED wsa_overlapped_;
    char buf[MAX_BUFSIZE];
    WSABUF wsa_buf_;
};

void worker_thread(HANDLE handle)
{
    DWORD bytes_transferred;
    ULONG_PTR completion_key;
    LPOVERLAPPED overlapped;

    bool result = GetQueuedCompletionStatus(
        handle, &bytes_transferred, &completion_key, &overlapped, INFINITE);
    ClientContext* client = reinterpret_cast<ClientContext*>(completion_key);

    if (!result || bytes_transferred == 0)
    {
        if (client)
        {
            closesocket(client->socket_client_);
            delete client;
        }
    }

    client->wsa_buf_.len = bytes_transferred;
    DWORD flags = 0;
    if (WSASend(client->socket_client_, &client->wsa_buf_, 1, NULL, flags,
                &client->wsa_overlapped_, NULL) == SOCKET_ERROR)
    {
        if (WSAGetLastError() != WSA_IO_PENDING)
        {
            closesocket(client->socket_client_);
            delete client;
        }
    }
}

int main()
{
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) return 1;

    HANDLE h_iocp =
        CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, MAX_WORKERTHREAD);
    if (h_iocp == NULL) return 1;

    std::vector<std::thread> worker_threads;
    for (DWORD i = 0; i < MAX_THREAD; ++i)
        worker_threads.emplace_back(worker_thread, h_iocp);

    SOCKET listen_socket =
        WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (listen_socket == INVALID_SOCKET) return 1;

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listen_socket, (SOCKADDR*)&server_addr, sizeof(server_addr)) ==
        SOCKET_ERROR)
        return 1;

    if (listen(listen_socket, SOMAXCONN) == SOCKET_ERROR) return 1;

    std::cout << "server ready\n";

    while (true)
    {
        SOCKET client_socket = accept(listen_socket, NULL, NULL);
        if (client_socket == INVALID_SOCKET) continue;

        ClientContext* client = new ClientContext{};
        client->socket_client_ = client_socket;
        client->wsa_buf_.buf = client->buf;
        client->wsa_buf_.len = MAX_BUFSIZE;

        CreateIoCompletionPort((HANDLE)client->socket_client_, h_iocp,
                               (ULONG_PTR)client, 0);

        DWORD flags = 0;
        if (WSARecv(client->socket_client_, &client->wsa_buf_, 1, NULL, &flags,
                    &client->wsa_overlapped_, NULL) == SOCKET_ERROR)
        {
            if (WSAGetLastError() != WSA_IO_PENDING)
            {
                closesocket(client->socket_client_);
                delete client;
            }
        }
    }

    for (auto& th : worker_threads) th.join();

    closesocket(listen_socket);
    CloseHandle(h_iocp);
    WSACleanup();

    return 0;
}