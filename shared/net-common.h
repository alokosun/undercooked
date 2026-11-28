#ifndef NET_COMMON_H
#define NET_COMMON_H

#include <vector>
#include <cstdint>
#include <cstddef>

#include "net-platform.h"
#include "serialization.h"
#include "protocol.h"

struct Frame {
  uint8_t type = 0;  // raw value; check is_known_tcp_type() before casting to TcpType
  std::vector<uint8_t> payload;
};

//TCP framing
bool build_frame(uint8_t type, const uint8_t* payload, size_t len, std::vector<uint8_t>& out);
//combines message type and payload into TCP frams, including length prefix
//helps receiver identify message boundaries

//TCP receiving
class FrameAccumulator {
    public:
        enum class Status { NeedMore, Frame, Error };
        void append(const uint8_t* data, size_t len); //adds newly received TCP bytes to internal buffer
        Status next(Frame& out); //extracts the next complete message from accumulated data
        void reset(); //clears accumulated data and error state
};

//TCP sending
class SendQueue {
    public:
        bool enqueue(const uint8_t* data, size_t len); //adds outgoing TCP data to a queue, stores messages that have not yet been completely sent
        IoResult flush(Socket& s); //attempts to send queued data and tracks how much has been transmitted
};

//UDP
struct UdpHeader {
  UdpType type = UdpType::Input;
  uint16_t seq = 0;   // sender's packet counter
  uint16_t ack = 0;   // newest seq received from the other side
  PlayerId player_id = InvalidPlayer;
};

enum class UdpCheck { Ok, TooSmall, TooLarge, BadProtocolId, UnknownType };
const char* to_string(UdpCheck c);

void write_udp_header(Buffer_Writer& w, const UdpHeader& h); //serialize UDP packet header into bytes
UdpCheck parse_udp_header(const uint8_t* data, size_t len, UdpHeader& out); //reads and validates header of an incoming UDP packet

#endif