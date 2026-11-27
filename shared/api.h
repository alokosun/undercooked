//Interface for client/server to packetmaker
#ifndef API_H
#define API_H

#include "protocol.h"
#include "GameSnapshot.h"

UDP_OrderSnapshot to_struct (const OrderSnapshot& s);
OrderSnapshot to_class (const UDP_OrderSnapshot& su);

UDP_StationSnapshot to_struct(const StationSnapshot& s);
StationSnapshot to_class(const UDP_StationSnapshot& su);

UDP_PlayerSnapshot to_struct(const PlayerSnapshot& s);
PlayerSnapshot to_class (const UDP_PlayerSnapshot& su);

UDP_Snapshot to_struct(const GameSnapshot&);
GameSnapshot to_class(const UDP_Snapshot& su);

uint8_t to_uint8(int i);
uint16_t to_uint16(int i);
int16_t to_int16(float f);

#endif