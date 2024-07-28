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
    // 创建服务器Socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
    // 设置监听
    if (listen(serverSocket, 5) == SOCKET_ERROR)
    {
        std::println("Server listen failed. Error code: {}", WSAGetLastError());
        return -1;
    }
    std::println("Listening for client...");
    // 等待客户端连接
    SOCKADDR_IN clientAddress;
    int len = sizeof(SOCKADDR);
    SOCKET clientSocket = accept(serverSocket, reinterpret_cast<SOCKADDR*>(&clientAddress), &len);
    if (clientSocket == INVALID_SOCKET)
    {
        std::println("Server accept failed. Error code: {}", WSAGetLastError());
        return -1;
    }
    char ip[16];
    std::println("Client {}:{} connect. Start sending data...", inet_ntop(AF_INET, &clientAddress.sin_addr, ip, 16),
        ntohs(clientAddress.sin_port));

    int i = 5;
    while (i-- > 0)
    {
        // 给客户端发送数据
        std::print("Send: ");
        char sendBuf[1024];
        gets_s(sendBuf, 1024);
        if (send(clientSocket, sendBuf, 1024, 0) == SOCKET_ERROR)
        {
            std::println("Server send data failed. Error code: {}", WSAGetLastError());
            return -1;
        }
        // 从客户端接收数据
        std::println("Waiting for data...");
        char receiveBuf[1024];
        if (recv(clientSocket, receiveBuf, 1024, 0) == SOCKET_ERROR)
        {
            std::println("Server receive data failed. Error code: {}", WSAGetLastError());
            return -1;
        }
        std::println("Data: {}", receiveBuf);
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
