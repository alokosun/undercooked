//Author: Grace
//Interface for client/server to network layer/packetmaker
//shit like... send_message. will call packetmaker and then network handler.

#ifndef API_H
#define API_H

#include "protocol.h"
#include "GameSnapshot.h"
#include "packetmaker.h"
#include "serialization.h"

const int MAX_PACKET_SIZE = 100;

uint8_t* broadcast_snapshot(const OrderSnapshot& s);

UDP_OrderSnapshot to_protocol (const OrderSnapshot& s);
OrderSnapshot from_protocol (const UDP_OrderSnapshot& su);

UDP_StationSnapshot to_protocol(const StationSnapshot& s);
StationSnapshot from_protocol(const UDP_StationSnapshot& su);

UDP_PlayerSnapshot to_protocol(const PlayerSnapshot& s);
PlayerSnapshot from_protocol (const UDP_PlayerSnapshot& su);

UDP_Snapshot to_protocol(const GameSnapshot&);
GameSnapshot from_protocol(const UDP_Snapshot& su);

uint8_t to_uint8(int i);
uint16_t to_uint16(int i);
int16_t to_int16(float f);

#endif
