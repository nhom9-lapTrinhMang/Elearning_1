#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

int main() {
    const int PORT = 13000;
    const char* SERVER_ADDR = "127.0.0.1";

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "Khong the khoi tao Winsock. Ma loi: " << result << std::endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Khong the tao socket. Ma loi: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_ADDR, &serverAddr.sin_addr);

    std::cout << "Dang ket noi toi may chu " << SERVER_ADDR << ":" << PORT << "..." << std::endl;

    result = connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        std::cerr << "Khong the ket noi. Ma loi: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Ket noi thanh cong!\n";

    while (true) {
        std::string message;
        std::cout << "Nhap tin nhan gui den may chu (Enter de thoat): ";
        std::getline(std::cin, message);

        if (message.empty()) {
            break; 
        }

        int bytesSent = send(clientSocket, message.c_str(), (int)message.size(), 0);
        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "Loi khi gui du lieu. Ma loi: " << WSAGetLastError() << std::endl;
            break;
        }

        char buffer[65536];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Phan hoi tu may chu: " << buffer << std::endl;
        }
        else if (bytesReceived == 0) {
            std::cout << "May chu da dong ket noi." << std::endl;
            break;
        }
        else {
            std::cerr << "Loi khi nhan du lieu. Ma loi: " << WSAGetLastError() << std::endl;
            break;
        }
    }

    std::cout << "🔚 Da ngat ket noi khoi may chu.\n";

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
