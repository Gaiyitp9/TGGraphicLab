#include <winsock2.h>
#include <ws2tcpip.h>
#include <print>

int main()
{
    // 初始化Windows Socket
    WSADATA wsadata;
    int error = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (error == 0)
        std::println("Open client socket success.");
    else
    {
        std::println("Open client socket failed. Error code: {}", error);
        return -1;
    }
    // 创建客户端socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET)
    {
        std::println("Create client socket failed. Error code: {}", WSAGetLastError());
        return -1;
    }
    // 服务器的地址信息
    SOCKADDR_IN server;
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    IN_ADDR ip;
    if (inet_pton(AF_INET, "127.0.0.1", &ip) == 0)
    {
        std::println("Ipv4 address convert failed. Error code: {}", WSAGetLastError());
        return -1;
    }
    server.sin_addr = ip;
    // 连接服务器
    if (connect(clientSocket, reinterpret_cast<SOCKADDR*>(&server), sizeof(SOCKADDR)) != 0)
    {
        std::println("Connect server failed. Error code: {}", WSAGetLastError());
        return -1;
    }
    std::println("Connect to server.");

    int i = 5;
    while (i-- > 0)
    {
        std::println("Waiting for data...");
        char receiveBuf[1024];
        // 从服务器接收数据
        if (recv(clientSocket, receiveBuf, 1024, 0) == SOCKET_ERROR)
        {
            std::println("Client receive data failed. Error code: {}", WSAGetLastError());
            return -1;
        }
        std::println("Data: {}", receiveBuf);

        // 向服务器发送数据
        std::print("Send: ");
        char sendBuf[1024];
        gets_s(sendBuf, 1024);
        if (send(clientSocket, sendBuf, 1024, 0) == SOCKET_ERROR)
        {
            std::println("Client send data failed. Error code: {}", WSAGetLastError());
            return -1;
        }
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
