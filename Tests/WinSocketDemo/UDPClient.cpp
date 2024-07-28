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
    // 创建客户端UDP socket
    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET)
    {
        std::println("Create client socket failed. Error code: {}", WSAGetLastError());
        return -1;
    }
    // 服务器的地址信息
    SOCKADDR_IN server;
    int len = sizeof(server);
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    IN_ADDR ip;
    if (inet_pton(AF_INET, "127.0.0.1", &ip) == 0)
    {
        std::println("ipv4 address convert failed. Error code: {}", WSAGetLastError());
        return -1;
    }
    server.sin_addr = ip;

    int i = 5;
    while (i-- > 0)
    {
        char sendBuf[512];
        std::print("Send: ");
        gets_s(sendBuf, 512);
        // 向服务器发送数据
        if (sendto(clientSocket, sendBuf, 512, 0, reinterpret_cast<SOCKADDR*>(&server), len) == SOCKET_ERROR)
        {
            error = WSAGetLastError();
            std::println("Client send data failed. Error code: {}", error);
            return -1;
        }

        std::println("Waiting for data...");
        char receiveBuf[512];
        // 从服务器接收数据
        if (recvfrom(clientSocket, receiveBuf, 512, 0, reinterpret_cast<SOCKADDR*>(&server), &len) == SOCKET_ERROR)
        {
            error = WSAGetLastError();
            std::println("Client receive data failed. Error code: {}", error);
            return -1;
        }
        std::println("Data: {}", receiveBuf);
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
