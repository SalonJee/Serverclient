#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdio>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080

// Custom implementation of inet_pton for IPv4
int inet_pton_custom(int af, const char* src, void* dst) {
    if (af == AF_INET) {
        struct in_addr addr;
        if (inet_addr(src) == INADDR_NONE) {
            return 0;  // Invalid address
        } else {
            addr.s_addr = inet_addr(src);
            memcpy(dst, &addr, sizeof(struct in_addr));
            return 1;  // Address converted
        }
    }
    return -1;  // AF not supported
}

int main() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cerr << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Socket creation error" << std::endl;
        WSACleanup();
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Use the custom inet_pton function for IP address conversion
    if (inet_pton_custom(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address / Address not supported" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    while (true) {
        std::string message;
        std::cout << "Client: ";
        std::getline(std::cin, message);
        send(sock, message.c_str(), message.size(), 0);
        memset(buffer, 0, sizeof(buffer));
        int valread = recv(sock, buffer, 1024, 0);
        if (valread > 0) {
            std::cout << "Server: " << buffer << std::endl;
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}


