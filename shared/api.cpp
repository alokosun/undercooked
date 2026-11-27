#include "api.h"

#include <cmath>

//ORDER SNAPSHOT to/from protocol
UDP_OrderSnapshot to_protocol (const OrderSnapshot& s) {
    return UDP_OrderSnapshot {
        to_uint8(s.orderId), 
        uint8_t(s.recipe), 
        uint16_t(s.timeRemaining), 
        s.completed, 
        s.expired
    };
}

OrderSnapshot from_protocol(const UDP_OrderSnapshot& su) {
    return OrderSnapshot {
        int(su.id),
        RecipeType(su.type),
        int(su.time_remaining),
        su.completed,
        su.expired
    };
}

//STATION SNAPSHOT to/from protocol
UDP_StationSnapshot to_protocol(const StationSnapshot& s) {
    return UDP_StationSnapshot {
        to_uint8(s.stationId),
        s.occupied,
        uint8_t(s.itemType),
        uint8_t(s.itemState),
        to_uint16(s.cookingTimeRemaining)
    };
}

StationSnapshot from_protocol(const UDP_StationSnapshot& su) {
    return StationSnapshot {
        int(su.id),
        su.occupied,
        ItemType(su.item_type),
        ItemState(su.item_state),
        int(su.cooking_time_remaining)
    };
}

// Converts the player's current game action into a bit flag for UDP transmission.
// Each action uses a separate bit in UDP_PlayerSnapshot.flags so the protocol
// can later support multiple simultaneous player states if needed.
uint8_t player_action_to_flags(PlayerAction action)
{
    switch (action)
    {
        case PlayerAction::Chopping:
            return kFlagChopping;

        case PlayerAction::Cooking:
            return kFlagCooking;

        case PlayerAction::Plating:
            return kFlagPlating;

        case PlayerAction::Serving:
            return kFlagServing;

        case PlayerAction::PickingUp:
            return kFlagPickup;

        case PlayerAction::Dropping:
            return kFlagDrop;

        case PlayerAction::Idle:
        default:
            return 0;
    }
}

// Player snapshot network encoding:
// UDP_PlayerSnapshot uses held = 0 to mean the player is holding nothing.
// Since ItemType::Lettuce is also enum value 0, item values are shifted by +1
// when sending over the network and shifted back by -1 when decoding.
// Facing is also copied directly from the PlayerSnapshot into the UDP packet.

// PLAYERSNAPSHOT to/from protocol
UDP_PlayerSnapshot to_protocol(const PlayerSnapshot& s)
{
    uint8_t held = 0;

    if (s.holdingItem)
    {
        held = static_cast<uint8_t>(s.heldItemType) + 1;
    }

    return UDP_PlayerSnapshot {
        to_uint8(s.playerId),
        to_int16(s.x),
        to_int16(s.y),
        uint8_t(s.facing),
        held,
        player_action_to_flags(s.currentAction)
    };
}

PlayerSnapshot from_protocol(const UDP_PlayerSnapshot& su)
{
    PlayerSnapshot snapshot{};

    snapshot.playerId = int(su.id);
    snapshot.x = float(su.x);
    snapshot.y = float(su.y);

    snapshot.holdingItem = (su.held != 0);

    if (snapshot.holdingItem)
    {
        snapshot.heldItemType =
            static_cast<ItemType>(su.held - 1);
    }

    snapshot.facing =
        static_cast<Direction>(su.facing);

    if (su.flags & kFlagChopping)
    {
        snapshot.currentAction = PlayerAction::Chopping;
    }
    else if (su.flags & kFlagCooking)
    {
        snapshot.currentAction = PlayerAction::Cooking;
    }
    else if (su.flags & kFlagPlating)
    {
        snapshot.currentAction = PlayerAction::Plating;
    }
    else if (su.flags & kFlagServing)
    {
        snapshot.currentAction = PlayerAction::Serving;
    }
    else if (su.flags & kFlagPickup)
    {
        snapshot.currentAction = PlayerAction::PickingUp;
    }
    else if (su.flags & kFlagDrop)
    {
        snapshot.currentAction = PlayerAction::Dropping;
    }
    else
    {
        snapshot.currentAction = PlayerAction::Idle;
    }

    return snapshot;
}

//SNAPSHOT to/from protocol
UDP_Snapshot to_protocol(const GameSnapshot& s) {
    UDP_Snapshot s_out;
    s_out.players[0] = to_protocol(s.player1);
    s_out.players[1] = to_protocol(s.player2);
    s_out.num_players = 2;
    
    uint8_t counter = 0;
    for (int i = 0; i < MAX_STATIONS; i++) {
        if (false) { //TODO: DETECT EMPTY/NON-RELEVANT STATIONS
            continue;
        }
        s_out.stations[i] = to_protocol(s.stations[i]);
        counter++;
    }
    s_out.num_stations = counter;

    counter = 0;
    for (int i = 0; i < MAX_ORDERS; i++) {
        if (false) {//TODO: DETECT EMPTY ORDERS
            continue;
        }
        s_out.orders[i] = to_protocol(s.orders[i]);
        counter++;
    }
    s_out.num_orders = counter;

    s_out.score = to_uint16(s.score);

    return s_out;
}

GameSnapshot from_protocol(const UDP_Snapshot& su) {
     GameSnapshot s;
     s.player1 = from_protocol(su.players[0]);
     s.player2 = from_protocol(su.players[1]);

     for (int i = 0; i < su.num_orders; i++) {
        s.orders[i] = from_protocol(su.orders[i]);
     }

     for (int i = 0; i < su.num_stations; i++) {
        s.stations[i] = from_protocol(su.stations[i]);
     }

     s.score = su.score;

     return s;
}

//UNIT CONVERSIONS
uint8_t to_uint8(int i) {
    if (i < 0) return 0;
    if (i > 255) return 255;
    return uint8_t(i);
}

uint16_t to_uint16(int i) {
    if (i < 0) return 0;
    if (i > 0xFFFF) return 0xFFFF;
    return uint16_t(i);
}

int16_t to_int16(float f) {
    f = std::round(f);
    //if (f < -32,767) return -32,767;
    //if (f > 32,767) return 32,767;
    return f;
}