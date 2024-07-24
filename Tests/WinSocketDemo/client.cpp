#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

int main()
{
    // 初始化Windows Socket
    WSADATA wsadata;
    int error = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (error == 0)
        std::cout << "Open client socket success." << std::endl;
    else
    {
        std::cout << "Open client socket failed: " << error << std::endl;
        return -1;
    }
    // 创建客户端socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        error = WSAGetLastError();
        std::cout << "Create client socket failed: " << error << std::endl;
        return -1;
    }
    // 服务器的地址信息
    SOCKADDR_IN server;
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    IN_ADDR ip;
    if (inet_pton(AF_INET, "127.0.0.1", &ip) == 0)
    {
        error = WSAGetLastError();
        std::cout << "ipv4 address convert failed: " << error << std::endl;
        return -1;
    }
    server.sin_addr = ip;
    // 连接服务器
    if (connect(clientSocket, reinterpret_cast<SOCKADDR*>(&server), sizeof(SOCKADDR)) != 0)
    {
        error = WSAGetLastError();
        std::cout << "Connect server failed: " << error << std::endl;
        return -1;
    }
    std::cout << "Connect to server. Waiting for data..." << std::endl;

    int i = 5;
    while (i-- > 0)
    {
        char sendBuf[1024];
        char receiveBuf[1024];
        // 从服务器接收数据
        if (recv(clientSocket, receiveBuf, 1024, 0) == SOCKET_ERROR)
        {
            error = WSAGetLastError();
            std::cout << "Client receive data failed: " << error << std::endl;
            return -1;
        }
        std::cout << "Receive: " << receiveBuf << std::endl;
        // 向服务器发送数据
        std::cout << "Send: ";
        gets_s(sendBuf, 1024);

        if (send(clientSocket, sendBuf, 1024, 0) == SOCKET_ERROR)
        {
            error = WSAGetLastError();
            std::cout << "Client send data failed: " << error << std::endl;
            return -1;
        }
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
