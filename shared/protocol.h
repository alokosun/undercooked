//Author: Claude ft Grace
//definitions of messages that could be sent

// TCP control channel. Frame = [u16 length][u8 type][payload]; length counts the
// type byte plus the payload (so length >= 1). Big-endian.
//
//  Type        Dir   Payload                                      Notes
//  Hello       C->S  u32 version, str name                        first message
//  Welcome     S->C  u8 id, u64 token, u16 udp_port, u8 tick_rate reply to Hello
//  Reject      S->C  u8 reason                                    then server closes
//  LobbyState  S->C  u8 level, u8 n, n * (id,name,ready,chef)     on every change
//  Ready       C->S  bool ready
//  Start       S->C  u8 w, u8 h, w*h tiles, u8 n, n*(id,x,y), u32 start_tick
//  Leave       C->S  (empty)                                      clean quit
//  PlayerLeft  S->C  u8 id, u8 reason
//  Ping/Pong   both  u32 nonce                                    heartbeat / RTT
//
// UDP game channel. Packet = [UdpHeader (10 bytes)][payload]. Loss is expected.
//
//  Type      Dir   Payload                                        Loss handling
//  UdpHello  C->S  u64 token                                      resent until acked
//  UdpAck    S->C  u8 player_id                                   resend triggers re-ack
//  Input     C->S  u8 n, n * (u32 tick, i8 mx, i8 my, u8 btns)    last N inputs repeated
//  Snapshot  S->C  u32 tick,u16 time_left,u16 score,u8 n,        full state; next one
//                  n * (id, i16 x, i16 y, facing, held, flags)    replaces a lost one
// ---------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "serialization.h"
#include "GameSnapshot.h"
#include "Player.h"
#include "Map.h"

const short PROTOCOL_VERSION = 1;
const short MAX_PLAYERS = 2;
const short MAX_ORDERS = 2;
const short MAX_STATIONS = 5;
const short INPUT_REDUNDANCY = 4;

using PlayerId = uint8_t;
using Token = uint64_t;
using Tick = uint32_t;
const PlayerId InvalidPlayer = 0xFF;

enum class TcpType : uint8_t {
  Hello = 1, Welcome = 2, Reject = 3, LobbyState = 4, Ready = 5, ChooseChef = 6,
  Start = 7, Leave = 8, PlayerLeft = 9, Ping = 10, Pong = 11,
};

enum class UdpType : uint8_t { 
  UdpHello = 1, UdpAck = 2, Input = 3, Snapshot = 4 
};

enum class RejectReason : uint8_t {
  VersionMismatch = 1, LobbyFull = 2, GameInProgress = 3, BadName = 4, DuplicateName = 5, Malformed = 6,
};

enum class LeaveReason : uint8_t { 
  Quit = 0, Timeout = 1, Kicked = 2, ConnectionLost = 3, ProtocolError = 4 
};

// Input button bits.
constexpr uint8_t kBtnInteract = 1 << 0;
constexpr uint8_t kBtnPickDrop = 1 << 1;

inline bool is_known_tcp_type(uint8_t t) { return t >= 1 && t <= 11; }
inline bool is_known_udp_type(uint8_t t) { return t >= 1 && t <= 4; }

const char* to_string(TcpType t);
const char* to_string(UdpType t);
const char* to_string(RejectReason r);
const char* to_string(LeaveReason r);

// 1 to max name length - printable ASCII characters (no control characters).
bool valid_name(const std::string& name);

// ---- TCP message payloads ----
struct TCP_Empty {};  // for messages with no payload (Leave)

struct TCP_Hello { 
  uint32_t version = PROTOCOL_VERSION; 
  std::string name; 
};

struct TCP_Welcome { 
  PlayerId id = InvalidPlayer; 
  Token token = 0; 
  uint16_t udp_port = 0; 
  uint8_t tick_rate = 0; 
};

struct TCP_Reject { 
  RejectReason reason = RejectReason::Malformed; 
};

struct TCP_LobbyPlayer { 
  PlayerId id = InvalidPlayer; 
  std::string name; 
  bool ready = false;
};

struct TCP_LobbyState {
  uint8_t level_id = 0;
  uint8_t count = 0;
  TCP_LobbyPlayer players[MAX_PLAYERS];
};

struct TCP_Ready { bool ready = false; };

struct TCP_Spawn { 
  PlayerId id = InvalidPlayer; 
  uint8_t x = 0, y = 0; 
};

struct TCP_Start {
  uint8_t width = 0, height = 0;
  std::vector<uint8_t> tiles;  // width * height, row-major; tile meaning is defined by the game
  uint8_t player_count = 0;
  TCP_Spawn spawns[MAX_PLAYERS];
  Tick start_tick = 0;
};

struct TCP_PlayerLeft { 
  PlayerId id = InvalidPlayer; 
  LeaveReason reason = LeaveReason::Quit; 
};

struct TCP_Ping { uint32_t nonce = 0; };

struct TCP_Pong { uint32_t nonce = 0; };

// ---- UDP message payloads ----
struct UDP_Hello { Token token = 0; };

struct UDP_Ack { PlayerId player_id = InvalidPlayer; };

struct UDP_InputState {
  Tick tick = 0;
  int8_t move_x = 0;  // -1, 0, +1
  int8_t move_y = 0;  // -1, 0, +1
  uint8_t buttons = 0;
};

struct UDP_InputPacket {
  uint8_t count = 0;  // 1..kInputRedundancy, newest first
  UDP_InputState inputs[INPUT_REDUNDANCY];
};

struct UDP_PlayerSnapshot {
  PlayerId id = InvalidPlayer;
  int16_t x = 0, y = 0;  // fixed point, see cfg::kPosScale
  uint8_t facing = 0;
  uint8_t held = 0;      // 0 = nothing
  uint8_t flags = 0;
};

struct UDP_OrderSnapshot {
    uint8_t id = 255;
    uint8_t type = 255;
    uint16_t time_remaining = 0;
    bool completed = false;
    bool expired = true;
};

struct UDP_StationSnapshot {
    uint8_t id = 255;
    bool occupied = false;
    uint8_t item_type = 255;
    uint8_t item_state = 255;
    uint16_t cooking_time_remaining = 0;
};

struct UDP_Snapshot {
  Tick tick = 0;
  uint16_t time_left = 0;
  uint16_t score = 0;

  uint8_t num_players = 0;
  UDP_PlayerSnapshot players[MAX_PLAYERS];
  uint8_t num_orders = 0;
  UDP_OrderSnapshot orders[MAX_ORDERS];
  uint8_t num_stations = 0;
  UDP_StationSnapshot stations[MAX_STATIONS];
};