#include "net-platform.h"

#include <cstring>
#include <unistd.h>

//error definitions
#ifdef _WIN32 //windows
    #define ERR(name) WSA##name
#else //linux
    #define ERR(name) name
#endif

static int last_err() {
    #ifdef _WIN32 //windows
        return WSAGetLastError();
    #else //linux
        return errno;
    #endif
}

//for tracking of initialization
bool INITIALIZED = false;

//Initialization
//initializes networking subsystem (mostly for Winsock...)
bool platform_init() {
    #ifdef _WIN32
        if (INITIALIZED) return false;
        WSADATA data;
        WSAStartup(MAKEWORD(2, 2), &data);
        INITIALIZED = true;
    #endif
    return true;
}
//closes winsock
void platform_shutdown() {
    #ifdef _WIN32
        if (!INITIALIZED) return;
        WSACleanup();
        INITIALIZED = false;
    #endif
}

//SOCKET MANAGEMENT
//makes ipv4 TCP socket
Socket tcp_create() {
    //DIFFERENT FOR WINDOWS
    Socket s = socket(
        AF_INET, //ipv4
        SOCK_STREAM, //TCP socket
        0 //id maybe?
    );
    return s;
}

Socket udp_create(); //makes a UDP socket

//assigns IP address and port to a socket
//binds socket to ipv4 on any interface at specified port
bool bind_port(Socket& s, uint16_t port) {
    if (!valid_socket(s)) return false;
    
    //DIFFERENT FOR WINDOWS (probably)
    sockaddr_in sock{};
    sock.sin_family = AF_INET; //ipv4
    sock.sin_port = htons(port); //the port.
    sock.sin_addr.s_addr = INADDR_ANY; //any address/interface
    return (bind(s, (struct sockaddr*)&sock, sizeof(sock)) == 0); //bind
}

//close a socket
void close_socket(Socket& s) { 
    #ifdef _WIN32 //windows
        closesocket(s);
    #else //linux
        close(s);
    #endif 
}

bool set_nonblocking(Socket& s); //configures a socket to return operations immediately.
//Prevents slow client or operation from freezing server
int poll_sockets(PollItem* items, size_t count, int timeout_ms); //checks multiple sockets to see which are ready to read or write

//TCP COMMUNICAION
//sends bytes through a TCP socket
IoResult tcp_send(Socket& s, const void* data, size_t len) {
    #ifdef _WIN32 //windows
        int bytes_out = send(s, (const char*) data, static_cast<int>(len), 0);
    #else //Linux (oh boi this is a trend)
        //flag MSG_NOSIGNAL prevents process from terminating when sending to a closed socket on Linux
        int bytes_out = static_cast<int>(send(s, data, len, MSG_NOSIGNAL)); //returns num bytes sent
    #endif
    if (bytes_out < 0) return fail_result(false); //error handling
    return IoResult{
        (bytes_out == 0 && len > 0) ? Result::WouldBlock : Result::Ok, //WouldBlock if nothing sent, ok if information sent
        bytes_out //number of bytes successfully sent
    };
}

//receives bytes from a TCP socket
IoResult tcp_recv(Socket& s, void* data, size_t len) {
    #ifdef _WIN32 //windows
        int bytes_received = recv(s, (char*) data, static_cast<int>(len), 0);
    #else //linux
        int bytes_received = static_cast<int>(recv(s, data, len, 0)); //returns num bytes received
    #endif

    if (bytes_received < 0) return fail_result(false); //error handling
    return IoResult {
        (bytes_received == 0 && len > 0) ? Result::Closed : Result::Ok, //closed if no bytes received, ok if bytes received
        bytes_received //number of bytes received
    };
} 

//TCP socket into listen mode. 
//Returns true if successful (connection found), false otherwise
bool listen_on(Socket& s, int backlog) {
    return (valid_socket(s) && listen(s, backlog) == 0);
}

//Accepts TCP connection from listener. Returns client socket and stores client address in peer
Socket accept_client(Socket& listener, Address* peer = nullptr, Result* res = nullptr) {
    sockaddr_in sock;
    SocketLength sock_length = sizeof(sock);
    std::memset(&sock, 0, sizeof(sock)); //initialize sock to 0
    Socket client = accept(
        listener, //listening socket
        reinterpret_cast<sockaddr*>(&sock), //typecast sockaddr_in to sockaddr
        &sock_length //sock length
    );
    //check for bad Socket
    if (client == BAD_SOCKET) {
        //return relevant error if requested
        if (res) *res = map_error(last_err(), false); //but actually go fetch an error
        return Socket{};
    }
    //return address if requested
    if (peer) *peer = sockaddr_to_Address(sock);
    //return result if requested
    if (res) *res = Result::Ok;

    return client;
}

//initiates TCP connection to remote address
IoResult connect_to(Socket& s, const Address& addr) {
    sockaddr_in sock = Address_to_sockaddr(addr);
    if (connect(
        s, //socket
        reinterpret_cast<sockaddr*>(&sock), //type cast to sockaddr
        sizeof(sock) //size of sockaddr_in
    ) != 0) {
        return fail_result(false);
    }

    return IoResult {Result::Ok};
}

//UDP COMMUNICATION
IoResult udp_send(Socket& s, const void* data, size_t len, const Address& to); //sends UDP datagram to address/port
IoResult udp_recv(Socket& s, void* buf, size_t cap, Address* from); //receives UDP datagram and identifies sender

//UTILITIES
//resolves hostname to ip address using inbuilt mDNS
bool resolve(const std::string& host, uint16_t port, Address& out);

//converts from Address to properly formatted string. 
//generated by Claude
std::string Address::to_string() const {
    char buf[32];
    std::snprintf(buf, sizeof(buf), "%u.%u.%u.%u:%u", unsigned((ip >> 24) & 255), unsigned((ip >> 16) & 255),
                    unsigned((ip >> 8) & 255), unsigned(ip & 255), unsigned(port));
    return buf;
}

//converts from address (host byte order) to sockaddr (network byte order)
//generated by Claude
sockaddr_in Address_to_sockaddr(const Address& a) {
  sockaddr_in sock;
  std::memset(&sock, 0, sizeof(sock)); //initialize sockaddr to 0
  sock.sin_family = AF_INET; //ipv4
  sock.sin_port = htons(a.port); //assign port
  sock.sin_addr.s_addr = htonl(a.ip); //assign ip address
  return sock;
}

//converts from sockaddr (network byte order) to Address (host byte order)
//generated by Claude
Address sockaddr_to_Address(const sockaddr_in& sock) {
  Address a;
  a.ip = ntohl(sock.sin_addr.s_addr); //extract sockaddr address
  a.port = ntohs(sock.sin_port); //extract sockaddr port
  return a;
}

// Translate an OS error into a normalized category. `udp` selects datagram semantics,
// where "connection reset/refused" style errors are harmless noise.
//Claude generated... for Linux only LOL
Result map_error(int e, bool udp) {
    if (e == ERR(EWOULDBLOCK) || e == ERR(EINTR) || e == ERR(EINPROGRESS)) return Result::WouldBlock;

    #ifndef _WIN32 //linux only
        if (e == EAGAIN) return Result::WouldBlock;
    #endif

    if (udp) {
        if (e == ERR(ECONNRESET) || e == ERR(ECONNREFUSED) || e == ERR(EMSGSIZE) || e == ERR(ENETUNREACH) ||
            e == ERR(EHOSTUNREACH) || e == ERR(ENETRESET)) {
            return Result::Ignorable;
        }
        return Result::Fatal;
    }

    if (e == ERR(ECONNRESET) || e == ERR(ECONNABORTED) || e == ERR(ETIMEDOUT) || e == ERR(ENETRESET) ||
      e == ERR(ENETUNREACH) || e == ERR(EHOSTUNREACH) || e == ERR(ECONNREFUSED)) {
        return Result::Reset;
    }
    #ifndef _WIN32 //linux only
        if (e == EPIPE) return Result::Reset;
    #endif
    return Result::Fatal;
}

//IoResult from last OS error
IoResult fail_result(bool udp) {
  IoResult r;
  r.os_error = last_err();
  r.result = map_error(r.os_error, udp);
  return r;
}