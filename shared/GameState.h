#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <vector>

#include "Map.h"
#include "Player.h"
#include "Station.h"

class GameState
{
private:
    Map kitchenMap;

    Player player1;
    Player player2;

    std::vector<Station> stations;

public:
    GameState();

    Player& getPlayer1();
    Player& getPlayer2();

    const Map& getMap() const;

    bool movePlayer(int playerId, int dx, int dy);

    Station* getStation(int stationId);

    bool placeItemAtStation(Item* item, int stationId);
    Item* removeItemFromStation(int stationId);

    bool chopAtStation(int playerId, int stationId);
    bool startCookingAtStation(int stationId);
    bool finishCookingAtStation(int stationId);

    bool isPlayerAdjacentToStation(int playerId, int stationId);
};

#endif