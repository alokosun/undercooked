//Author: Grace with ChatGPT assistance

#ifndef NET_PLATFORM_H
#define NET_PLATFORM_H

#include <string>
#include <cstdint>
#include <cstddef>
#include <chrono>
#include <netinet/in.h>

static uint16_t TCP_PORT = 8005;
static uint16_t UDP_PORT = 8006;

//DIFFERENT FOR WINDOWS
using Socket = int;
inline bool valid_socket(Socket s) {return s >= 0;};

enum class Result { Ok, WouldBlock, Closed, Reset, Ignorable, Fatal };

struct IoResult {
  Result result = Result::Fatal;
  int bytes = 0;     // valid when result == Ok
  int os_error = 0;  // raw OS error for logging only; never branch on it
};

// IPv4 endpoint, both fields in HOST byte order. 127.0.0.1 == 0x7F000001.
struct Address {
  uint32_t ip = 0;
  uint16_t port = 0;
  bool valid() const { return ip != 0 || port != 0; }
  bool operator==(const Address& o) const { return ip == o.ip && port == o.port; }
  bool operator!=(const Address& o) const { return !(*this == o); }
  std::string to_string() const;  // "a.b.c.d:port"
};

/*
// Opaque socket handle. Copyable; the owner calls close_socket() exactly once.
struct Socket {
  int64_t handle = -1;
  bool valid() const { return handle >= 0; }
};
*/

//socket status
struct PollItem {
  Socket sock;
  bool want_read = false;
  bool want_write = false;
  bool can_read = false;   // outputs
  bool can_write = false;
  bool error = false;
};

//Initialization
bool platform_init(); //initializes networking subsystem (Winsock initialization...)
void platform_shutdown(); //housekeeping

//socket management
Socket tcp_create(); //makes a TCP socket
Socket udp_create(); //makes a UDP socket
bool bind_port(Socket& s, uint16_t port); //assigns IP address and port to a socket
void close_socket(Socket& s); //close a socket
bool set_nonblocking(Socket& s); //configures a socket to return operations immediately.
//Prevents slow client or operation from freezing server
int poll_sockets(PollItem* items, size_t count, int timeout_ms); //checks multiple sockets to see which are ready to read or write

//TCP COMMUNICAION
IoResult tcp_send(Socket& s, const void* data, size_t len); //sends bytes through a TCP socket
IoResult tcp_recv(Socket& s, void* buf, size_t cap); //receives bytes from a TCP socket
bool listen_on(Socket& s, int backlog); //TCP socket into listen mode
Socket accept_client(Socket& listener, Address* peer, Result* res); //Accepts incoming TCP connection and returns new socket for that client
IoResult connect_to(Socket& s, const Address& addr); //initiates TCP connection to remote address

//UDP COMMUNICATION
IoResult udp_send(Socket& s, const void* data, size_t len, const Address& to); //sends UDP datagram to address/port
IoResult udp_recv(Socket& s, void* buf, size_t cap, Address* from); //receives UDP datagram and identifies sender

//UTILITIES
bool resolve(const std::string& host, uint16_t port, Address& out); //resolves hostname to address
sockaddr_in Address_to_sockaddr(const Address& a);
Address sockaddr_to_Address(const sockaddr_in& sock);
Result map_error(int e, bool udp); //turn system error into program error
IoResult fail_result(bool udp);

//get current time (for logging... perhaps put this into the logging file lmao)
inline uint64_t now_ms() {
  return static_cast<uint64_t>(
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch())
          .count());
};

#endif