//Author: Alyssa
//Struct definitions of Snapshots to be used by backend

#ifndef GAMESNAPSHOT_H
#define GAMESNAPSHOT_H

#include "Player.h"

struct PlayerSnapshot
{
    int playerId;
    float x;
    float y;
    bool holdingItem;
    ItemType heldItemType;
    
    Direction facing = Direction::Down;
    PlayerAction currentAction = PlayerAction::Idle;
};

struct StationSnapshot
{
    int stationId;
    bool occupied;
    ItemType itemType;
    ItemState itemState;
    int cookingTimeRemaining;
};

struct OrderSnapshot
{
    int orderId;
    RecipeType recipe;
    int timeRemaining;
    bool completed;
    bool expired;
};

struct GameSnapshot
{
    PlayerSnapshot player1;
    PlayerSnapshot player2;

    StationSnapshot stations[5];
    OrderSnapshot orders[2];

    int score;
};

#endif