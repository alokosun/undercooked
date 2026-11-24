// laptop_sender_windows.cpp - Windows version (Winsock2)
//
// Build with MSVC (Developer Command Prompt):
//   cl /EHsc /std:c++17 laptop_sender_windows.cpp
// Build with MinGW-w64:
//   g++ -std=c++17 -o laptop_sender_windows.exe laptop_sender_windows.cpp -lws2_32
//
// Run:   laptop_sender_windows.exe [host] [port]
//        host defaults to 169.254.67.67 (you can pass RPI-imposter.local instead)
//        port defaults to 5005
// Type a line and press Enter to send it. Ctrl+Z then Enter (or Ctrl+C) to quit.

#include <winsock2.h>
#include <ws2tcpip.h>

#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")  // MSVC only; MinGW uses -lws2_32

int main(int argc, char* argv[]) {
    const char* host = (argc > 1) ? argv[1] : "169.254.243.31";
    const std::string port = (argc > 2) ? argv[2] : "5005";

    WSADATA wsa;
    int rc = WSAStartup(MAKEWORD(2, 2), &wsa);
    if (rc != 0) {
        std::cerr << "WSAStartup failed: " << rc << "\n";
        return 1;
    }

    // Resolve hostname or IP (IPv4 only, UDP)
    addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    rc = getaddrinfo(host, port.c_str(), &hints, &res);
    if (rc != 0) {
        std::cerr << "Could not resolve " << host << " (error " << rc << ")\n";
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

    std::cout << "Sending to " << host << ":" << port
              << " - type a message and press Enter.\n";

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        int sent = sendto(sock, line.data(), static_cast<int>(line.size()), 0,
                          res->ai_addr, static_cast<int>(res->ai_addrlen));
        if (sent == SOCKET_ERROR) {
            std::cerr << "sendto failed: " << WSAGetLastError() << "\n";
        }
    }

    closesocket(sock);
    freeaddrinfo(res);
    WSACleanup();
    return 0;
}