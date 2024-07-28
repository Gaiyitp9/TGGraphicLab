#include <winsock2.h>
#include <ws2tcpip.h>
#include <print>

int main()
{
    // 初始化Windows Socket
    WSADATA wsadata;
    int error = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (error == 0)
        std::println("Open server socket success.");
    else
    {
        std::println("Open server socket failed. Error code: {}", error);
        return -1;
    }
    // 创建服务器UDP Socket
    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET)
    {
        std::println("Create server socket failed. Error code: {}", WSAGetLastError());
        return -1;
    }
    // 设置服务器地址信息
    SOCKADDR_IN address;
    address.sin_family = AF_INET;
    address.sin_port = htons(12345);
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    // 绑定
    if (bind(serverSocket, reinterpret_cast<SOCKADDR*>(&address), sizeof(address)) == SOCKET_ERROR)
    {
        std::println("Server socket bind address failed. Error code: {}", WSAGetLastError());
        return -1;
    }

    int i = 5;
    while (i-- > 0)
    {
        std::println("Waiting for data...");

        // 从客户端接收数据
        char receiveBuf[512];
        SOCKADDR_IN clientAddress;
        int len = sizeof(clientAddress);
        if (recvfrom(serverSocket, receiveBuf, 512, 0, reinterpret_cast<SOCKADDR*>(&clientAddress), &len) == SOCKET_ERROR)
        {
            std::println("Server receive data failed. Error code: {}", WSAGetLastError());
            return -1;
        }
        char ip[16];
        std::println("Receive packet data from: {}:{}", inet_ntop(AF_INET, &clientAddress.sin_addr, ip, 16),
            ntohs(clientAddress.sin_port));
        std::println("Data: {}", receiveBuf);

        // 给客户端发送数据
        std::print("Send: ");
        char sendBuf[512];
        gets_s(sendBuf, 512);
        if (sendto(serverSocket, sendBuf, 512, 0, reinterpret_cast<SOCKADDR*>(&clientAddress), len) == SOCKET_ERROR)
        {
            std::println("Server send data failed. Error code: {}", WSAGetLastError());
            return -1;
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
