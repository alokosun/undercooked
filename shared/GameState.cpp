#include "GameState.h"

GameState::GameState()
    : player1(1, 1.0f, 1.0f),
      player2(2, 8.0f, 6.0f)
{
}

Player& GameState::getPlayer1()
{
    return player1;
}

Player& GameState::getPlayer2()
{
    return player2;
}

const Map& GameState::getMap() const
{
    return kitchenMap;
}

bool GameState::movePlayer(int playerId, int dx, int dy)
{
    Player* player = nullptr;

    if (playerId == 1)
    {
        player = &player1;
    }
    else if (playerId == 2)
    {
        player = &player2;
    }
    else
    {
        return false;
    }

    int newX = static_cast<int>(player->getX()) + dx;
    int newY = static_cast<int>(player->getY()) + dy;

    if (!kitchenMap.isWalkable(newX, newY))
    {
        return false;
    }

    // Prevent both players from occupying the same tile.
    Player& otherPlayer = (playerId == 1) ? player2 : player1;

    if (newX == static_cast<int>(otherPlayer.getX()) &&
        newY == static_cast<int>(otherPlayer.getY()))
    {
        return false;
    }

    player->setPosition(
        static_cast<float>(newX),
        static_cast<float>(newY)
    );

    return true;
}