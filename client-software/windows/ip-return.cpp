#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

static const char* PORT = "6767";

int main(int argc, char* argv[]) {
    const char* host = (argc > 1) ? argv[1] : "rpi-imposter.local";
    const char* msg  = (argc > 2) ? argv[2] : "ping";

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    if (getaddrinfo(host, PORT, &hints, &res) != 0) {
        std::cerr << "Could not resolve " << host
                  << " (try passing the IP instead)\n";
        WSACleanup();
        return 1;
    }

    SOCKET sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket failed: " << WSAGetLastError() << "\n";
        freeaddrinfo(res);
        WSACleanup();
        return 1;
    }

    DWORD timeout_ms = 3000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,
               reinterpret_cast<const char*>(&timeout_ms), sizeof timeout_ms);

    if (sendto(sock, msg, static_cast<int>(strlen(msg)), 0,
               res->ai_addr, static_cast<int>(res->ai_addrlen)) == SOCKET_ERROR) {
        std::cerr << "sendto failed: " << WSAGetLastError() << "\n";
    } else {
        char buf[1024];
        int n = recvfrom(sock, buf, sizeof buf - 1, 0, nullptr, nullptr);
        if (n == SOCKET_ERROR) {
            std::cerr << "No reply (timed out or error "
                      << WSAGetLastError() << ")\n";
        } else {
            buf[n] = '\0';
            std::cout << "Pi's eth0 IPv4: " << buf << "\n";
        }
    }

    freeaddrinfo(res);
    closesocket(sock);
    WSACleanup();
    return 0;
}