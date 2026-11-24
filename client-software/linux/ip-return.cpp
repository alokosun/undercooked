#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>

static const char* HOSTNAME = "rpi-imposter.local";
static const char* PORT     = "6767";

int main(int argc, char* argv[]) {
    const char* msg = (argc > 1) ? argv[1] : "ping";

    // Resolve the hostname to an IPv4 address
    addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;

    int rc = getaddrinfo(HOSTNAME, PORT, &hints, &res);
    if (rc != 0) {
        std::cerr << "Could not resolve " << HOSTNAME << ": "
                  << gai_strerror(rc) << "\n";
        return 1;
    }

    char resolved[INET_ADDRSTRLEN];
    auto* sin = reinterpret_cast<sockaddr_in*>(res->ai_addr);
    inet_ntop(AF_INET, &sin->sin_addr, resolved, sizeof resolved);
    std::cout << HOSTNAME << " resolved to " << resolved << "\n";

    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0) {
        perror("socket");
        freeaddrinfo(res);
        return 1;
    }

    // 3 second receive timeout
    timeval tv{};
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);

    if (sendto(sock, msg, std::strlen(msg), 0,
               res->ai_addr, res->ai_addrlen) < 0) {
        perror("sendto");
        close(sock);
        freeaddrinfo(res);
        return 1;
    }

    char buf[1024];
    ssize_t n = recvfrom(sock, buf, sizeof buf - 1, 0, nullptr, nullptr);
    if (n < 0) {
        perror("recvfrom (no reply)");
        close(sock);
        freeaddrinfo(res);
        return 1;
    }
    buf[n] = '\0';
    std::cout << "Pi's eth0 IPv4: " << buf << "\n";

    close(sock);
    freeaddrinfo(res);
    return 0;
}