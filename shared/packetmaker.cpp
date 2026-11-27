#include "packetmaker.h"

//GAME SNAPSHOT
void encode (Buffer_Writer& w, const UDP_Snapshot& s) {
    w.u32(s.tick);
    w.u16(s.time_left);
    w.u16(s.score);
    w.u8(s.num_players);
    for (int i = 0; i < s.num_players; i++) {
        encode(w, s.players[i]);
    }
    w.u8(s.num_orders);
    for (int i = 0; i < s.num_orders; i++) {
        encode(w, s.orders[i]);
    }
    w.u8(s.num_stations);
    for (int i = 0; i < s.num_stations; i++) {
        encode(w, s.stations[i]);
    }
}
void decode(Buffer_Reader& r, UDP_Snapshot& s) {
    s.tick = r.u32();
    s.time_left = r.u16();
    s.score = r.u16();

    s.num_players = r.u8();
    for (int i = 0; i < s.num_players; i++) {
        decode(r, s.players[i]);
    }

    s.num_orders = r.u8();
    for (int i = 0; i < s.num_orders; i++) {
        decode(r, s.orders[i]);
    }

    s.num_stations = r.u8();
    for (int i = 0; i < s.num_stations; i++) {
        decode(r, s.stations[i]);
    }
}


//PLAYER SNAPSHOT
void encode(Buffer_Writer& w, const UDP_PlayerSnapshot& s) {
    w.u8(s.id);
    w.i16(s.x);
    w.i16(s.y);
    w.u8(s.facing);
    w.u8(s.held);
    w.u8(s.flags);
}
void decode (Buffer_Reader& r, UDP_PlayerSnapshot& s) {
    s.id = r.u8();
    s.x = r.i16();
    s.y = r.i16();
    s.facing = r.u8();
    s.held = r.u8();
    s.flags = r.u8();
}

//ORDER SNAPSHOT
void encode(Buffer_Writer& w, const UDP_OrderSnapshot& s) {
    w.u8(s.id);
    w.u8(s.type);
    w.u16(s.time_remaining);
    w.boolean(s.completed);
    w.boolean(s.expired);
}
void decode(Buffer_Reader& r, UDP_OrderSnapshot& s) {
    s.id = r.u8();
    s.type = r.u8();
    s.time_remaining = r.u16();
    s.completed = r.boolean();
    s.expired = r.boolean();
}

//STATION SNAPSHOT
void encode(Buffer_Writer& w, const UDP_StationSnapshot& s) {
    w.u8(s.id);
    w.boolean(s.occupied);
    w.u8(s.item_type);
    w.u8(s.item_state);
    w.u16(s.cooking_time_remaining);
}
void decode(Buffer_Reader& r, UDP_StationSnapshot& s) {
    s.id = r.u8();
    s.occupied = r.boolean();
    s.item_type = r.u8();
    s.item_state = r.u8();
    s.cooking_time_remaining = r.u16();
}