#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Map.h"
#include "Player.h"

class GameState
{
private:
    Map kitchenMap;
    Player player1;
    Player player2;

public:
    GameState();

    Player& getPlayer1();
    Player& getPlayer2();

    const Map& getMap() const;

    bool movePlayer(int playerId, int dx, int dy);
};

#endif