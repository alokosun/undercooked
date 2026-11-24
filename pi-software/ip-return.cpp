#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>

static const int PORT = 6767;

// Returns eth0's IPv4 address as a string, or "" if not found
static std::string get_eth0_ipv4() {
    ifaddrs* ifaddr = nullptr;
    if (getifaddrs(&ifaddr) == -1) return "";

    std::string result;
    for (ifaddrs* ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr || ifa->ifa_addr->sa_family != AF_INET) continue;
        if (std::strcmp(ifa->ifa_name, "eth0") != 0) continue;

        char buf[INET_ADDRSTRLEN];
        auto* sin = reinterpret_cast<sockaddr_in*>(ifa->ifa_addr);
        inet_ntop(AF_INET, &sin->sin_addr, buf, sizeof buf);
        result = buf;
        break;
    }
    freeifaddrs(ifaddr);
    return result;
}

int main() {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) { perror("socket"); return 1; }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);

    if (bind(sock, reinterpret_cast<sockaddr*>(&addr), sizeof addr) < 0) {
        perror("bind");
        close(sock);
        return 1;
    }

    std::cout << "rpi-imposter listening on UDP port " << PORT << "...\n";

    char buf[1024];
    while (true) {
        sockaddr_in sender{};
        socklen_t sender_len = sizeof sender;

        ssize_t n = recvfrom(sock, buf, sizeof buf - 1, 0,
                             reinterpret_cast<sockaddr*>(&sender), &sender_len);
        if (n < 0) { perror("recvfrom"); continue; }
        buf[n] = '\0';

        char sender_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &sender.sin_addr, sender_ip, sizeof sender_ip);
        std::cout << "Received \"" << buf << "\" from " << sender_ip
                  << ":" << ntohs(sender.sin_port) << "\n";

        std::string reply = get_eth0_ipv4();
        if (reply.empty()) reply = "ERROR: eth0 has no IPv4 address";

        // Reply to the sender's IP and source port
        sendto(sock, reply.c_str(), reply.size(), 0,
               reinterpret_cast<sockaddr*>(&sender), sender_len);
        std::cout << "Replied with: " << reply << "\n";
    }

    close(sock);
    return 0;
}