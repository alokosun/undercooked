// pi_listener.cpp - runs on the Raspberry Pi
// Build: g++ -std=c++17 -o pi_listener pi_listener.cpp
// Run:   ./pi_listener [port]      (default port 5005)

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <iostream>

int main(int argc, char* argv[]) {
    const int port = (argc > 1) ? std::atoi(argv[1]) : 5005;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);  // listen on all interfaces
    addr.sin_port = htons(port);

    if (bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
        perror("bind");
        close(sock);
        return 1;
    }

    std::cout << "Listening for UDP packets on port " << port << "...\n";

    char buf[2048];
    while (true) {
        sockaddr_in sender{};
        socklen_t sender_len = sizeof(sender);

        ssize_t n = recvfrom(sock, buf, sizeof(buf) - 1, 0,
                             reinterpret_cast<sockaddr*>(&sender), &sender_len);
        if (n < 0) {
            perror("recvfrom");
            break;
        }
        buf[n] = '\0';

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &sender.sin_addr, ip, sizeof(ip));

        std::cout << "[" << ip << ":" << ntohs(sender.sin_port) << "] "
                  << buf << std::endl;
    }

    close(sock);
    return 0;
}