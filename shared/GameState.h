#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "Order.h"

#include <vector>

#include "Map.h"
#include "Player.h"
#include "Station.h"
#include "GameSnapshot.h"

enum class ActionType
{
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Pickup,
    Drop,
    Chop,
    Cook,
    Plate,
    Serve
};

class GameState
{
private:
    Map kitchenMap;

    Player player1;
    Player player2;

    std::vector<Station> stations;

    std::vector<Order> orders;

    int score;

public:
    GameState();

    Player& getPlayer1();
    Player& getPlayer2();

    const Map& getMap() const;

    bool movePlayer(int playerId, int dx, int dy);

    Station* getStation(int stationId);

    void addOrder(RecipeType recipe, int timeLimit);
    void updateOrders(int seconds);

    const std::vector<Order>& getOrders() const;

    RecipeType checkPlateRecipe(int stationId);

    bool placeItemAtStation(Item* item, int stationId);
    Item* removeItemFromStation(int stationId);

    bool chopAtStation(int playerId, int stationId);
    bool startCookingAtStation(int playerId, int stationId);
    bool finishCookingAtStation(int stationId);

    bool isPlayerAdjacentToStation(int playerId, int stationId);

    bool pickupFromStation(int playerId, int stationId);
    bool dropAtStation(int playerId, int stationId);

    bool plateItem(int playerId, int stationId);

    void updateCookingTimers(int seconds);

    bool serveOrder(int playerId, int stationId);
    int getScore() const;

    bool handleAction(int playerId, ActionType action, int targetId = -1);

    GameSnapshot getSnapshot() const;
};

#endif