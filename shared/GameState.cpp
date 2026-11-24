#include <cstdlib>
#include "GameState.h"

GameState::GameState()
    : player1(1, 1.0f, 1.0f),
      player2(2, 8.0f, 6.0f)
{
    stations.emplace_back(1, StationType::IngredientStation, 2, 2);
    stations.emplace_back(2, StationType::ChoppingStation, 4, 2);
    stations.emplace_back(3, StationType::Stove, 6, 2);
    stations.emplace_back(4, StationType::PlateStation, 6, 4);
    stations.emplace_back(5, StationType::ServingStation, 7, 4);
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

Station* GameState::getStation(int stationId)
{
    for (Station& station : stations)
    {
        if (station.getId() == stationId)
        {
            return &station;
        }
    }

    return nullptr;
}

bool GameState::placeItemAtStation(Item* item, int stationId)
{
    Station* station = getStation(stationId);

    if (station == nullptr)
    {
        return false;
    }

    return station->placeItem(item);
}

Item* GameState::removeItemFromStation(int stationId)
{
    Station* station = getStation(stationId);

    if (station == nullptr)
    {
        return nullptr;
    }

    return station->removeItem();
}

bool GameState::chopAtStation(int playerId, int stationId)
{
    if (!isPlayerAdjacentToStation(playerId, stationId))
    {
        return false;
    }

    Station* station = getStation(stationId);

    if (station == nullptr)
    {
        return false;
    }

    return station->chopItem();
}

bool GameState::startCookingAtStation(int stationId)
{
    Station* station = getStation(stationId);

    if (station == nullptr)
    {
        return false;
    }

    return station->startCooking();
}

bool GameState::finishCookingAtStation(int stationId)
{
    Station* station = getStation(stationId);

    if (station == nullptr)
    {
        return false;
    }

    return station->finishCooking();
}

bool GameState::isPlayerAdjacentToStation(int playerId, int stationId)
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

    Station* station = getStation(stationId);

    if (station == nullptr)
    {
        return false;
    }

    int playerX = static_cast<int>(player->getX());
    int playerY = static_cast<int>(player->getY());

    int dx = std::abs(playerX - station->getX());
    int dy = std::abs(playerY - station->getY());

    return (dx + dy) == 1;
}