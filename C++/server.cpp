#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

const int PORT = 13000;
const int BUFFER_SIZE = 65536;

void XuLyMayKhach(SOCKET clientSocket) {
    int flag = 1;
    setsockopt(clientSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));

    int size = BUFFER_SIZE;
    setsockopt(clientSocket, SOL_SOCKET, SO_RCVBUF, (char*)&size, sizeof(size));
    setsockopt(clientSocket, SOL_SOCKET, SO_SNDBUF, (char*)&size, sizeof(size));

    int keepAlive = 1;
    setsockopt(clientSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&keepAlive, sizeof(keepAlive));

    std::cout << "May khach da ket noi. Socket da duoc thiet lap." << std::endl;

    char buffer[BUFFER_SIZE];
    int bytesReceived;

    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytesReceived] = '\0';
        std::string message(buffer);
        std::cout << "May chu nhan duoc: " << message << std::endl;

        std::string response = "Da nhan: " + message;
        send(clientSocket, response.c_str(), (int)response.size(), 0);
    }

    std::cout << "May khach da ngat ket noi." << std::endl;
    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "Khong the khoi tao Winsock. Ma loi: " << result << std::endl;
        return 1;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Khong the tao socket lang nghe. Ma loi: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Loi bind. Ma loi: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Loi listen. Ma loi: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "May chu dang lang nghe tai cong " << PORT << "..." << std::endl;

    while (true) {
        SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Loi accept. Ma loi: " << WSAGetLastError() << std::endl;
            continue;
        }
        std::thread(XuLyMayKhach, clientSocket).detach();
    }

    closesocket(listenSocket);
    WSACleanup();
    return 0;
}
