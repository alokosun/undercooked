//Author: Grace
//Functions that make packets from protocol defined structs
//TODO: make class that will construct and deconstruct packets
//      for various messgaes and handle buffer reading/writing
//encode/decodes will be private, and some kinda public interface will exist

#ifndef PACKETMAKER_H
#define PACKETMAKER_H

#include "protocol.h"
#include "serialization.h"

//snapshot
void encode(Buffer_Writer& w, const UDP_Snapshot& s);
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