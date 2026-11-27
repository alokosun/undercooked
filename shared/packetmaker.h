//Functions that make packets
#ifndef PACKETMAKER_H
#define PACKETMAKER_H

#include "protocol.h"
#include "serialization.h"
#include "GameSnapshot.h"
#include "Player.h"

//snapshot
void encode (Buffer_Writer& w, const UDP_Snapshot& s);
void decode(Buffer_Reader& r, UDP_Snapshot& s);

//Player snapshot
void encode(Buffer_Writer& w, const UDP_PlayerSnapshot& s);
void decode (Buffer_Reader& r, UDP_PlayerSnapshot& s);

//Order Snapshot
void encode(Buffer_Writer& w, const UDP_OrderSnapshot& s);
void decode(Buffer_Reader& r, UDP_OrderSnapshot& s);

//Station Snapshot
void encode(Buffer_Writer& w, const UDP_StationSnapshot& s);
void decode(Buffer_Reader& r, UDP_StationSnapshot& s);

#endif