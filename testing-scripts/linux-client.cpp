
// laptop_sender_linux.cpp - Linux UDP sender
//
// Compile:
//   g++ -Wall -Wextra -std=c++17 -o laptop_sender laptop_sender_linux.cpp
//
// Run:
//   ./laptop_sender [host] [port]
//
// Defaults:
//   host = 169.254.243.31
//   port = 5005
//
// Type a line and press Enter to send it.
// Ctrl+D or Ctrl+C to quit.

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    const char* host = (argc > 1) ? argv[1] : "169.254.243.31";
    const std::string port = (argc > 2) ? argv[2] : "5005";

    // Resolve hostname or IP address (IPv4 UDP)
    addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

    int rc = getaddrinfo(host, port.c_str(), &hints, &res);
    if (rc != 0) {
        std::cerr << "Could not resolve " << host
                  << ": " << gai_strerror(rc) << "\n";
        return 1;
    }

    // Create UDP socket
    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0) {
        perror("socket");
        freeaddrinfo(res);
        return 1;
    }

    std::cout << "Sending to " << host << ":" << port
              << " - type a message and press Enter.\n";

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        ssize_t sent = sendto(
            sock,
            line.data(),
            line.size(),
            0,
            res->ai_addr,
            res->ai_addrlen
        );

        if (sent < 0) {
            perror("sendto");
        } else if (static_cast<size_t>(sent) != line.size()) {
            std::cerr << "Warning: incomplete UDP datagram sent.\n";
        }
    }

    close(sock);
    freeaddrinfo(res);
    return 0;
}
