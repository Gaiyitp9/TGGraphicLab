#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

int main()
{
    // 初始化Windows Socket
    WSADATA wsadata;
    int error = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (error == 0)
        std::cout << "Open server socket success." << std::endl;
    else
    {
        std::cout << "Open server socket failed: " << error << std::endl;
        return -1;
    }
    // 创建服务器Socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        error = WSAGetLastError();
        std::cout << "Create server socket failed: " << error << std::endl;
        return -1;
    }
    // 设置服务器地址信息
    SOCKADDR_IN address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8888);
    address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    // 绑定
    if (bind(serverSocket, reinterpret_cast<SOCKADDR*>(&address), sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        error = WSAGetLastError();
        std::cout << "Server socket bind address failed: " << error << std::endl;
        return -1;
    }
    // 设置监听
    if (listen(serverSocket, 5) == SOCKET_ERROR)
    {
        error = WSAGetLastError();
        std::cout << "Server listen failed: " << error << std::endl;
        return -1;
    }
    std::cout << "Listening for client..." << std::endl;
    // 等待客户端连接
    SOCKADDR_IN clientAddress;
    int len = sizeof(SOCKADDR);
    SOCKET clientSocket = accept(serverSocket, reinterpret_cast<SOCKADDR*>(&clientAddress), &len);
    if (clientSocket == INVALID_SOCKET)
    {
        error = WSAGetLastError();
        std::cout << "Server accept failed: " << error << std::endl;
        return -1;
    }
    std::cout << "A client connect. Start sending data..." << std::endl;

    int i = 5;
    while (i-- > 0)
    {
        // 给客户端发送数据
        std::cout << "Send: ";
        char sendBuf[1024];
        gets_s(sendBuf, 1024);
        if (send(clientSocket, sendBuf, 1024, 0) == SOCKET_ERROR)
        {
            error = WSAGetLastError();
            std::cout << "Server send data failed: " << error << std::endl;
            return -1;
        }
        // 从客户端接收数据
        char receiveBuf[1024];
        if (recv(clientSocket, receiveBuf, 1024, 0) == SOCKET_ERROR)
        {
            error = WSAGetLastError();
            std::cout << "Server receive data failed: " << error << std::endl;
            return -1;
        }
        std::cout << "Receive: " << receiveBuf << std::endl;
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
