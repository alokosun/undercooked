#include <cstdlib>
#include "GameState.h"

GameState::GameState()
    : player1(1, 1.0f, 1.0f),
      player2(2, 8.0f, 6.0f),
      score(0)
{
    stations.emplace_back(1, StationType::IngredientStation, 2, 2);
    stations.emplace_back(2, StationType::ChoppingStation, 4, 2);
    stations.emplace_back(3, StationType::Stove, 6, 2);
    stations.emplace_back(4, StationType::PlateStation, 6, 4);
    stations.emplace_back(5, StationType::ServingStation, 7, 4);

    orders.emplace_back(1, RecipeType::Salad, 60);
    orders.emplace_back(2, RecipeType::Burger, 90);
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

bool GameState::startCookingAtStation(int playerId, int stationId)
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

bool GameState::pickupFromStation(int playerId, int stationId)
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

    if (!isPlayerAdjacentToStation(playerId, stationId))
    {
        return false;
    }

    if (player->isHoldingItem())
    {
        return false;
    }

    Station* station = getStation(stationId);

    if (station == nullptr || !station->isOccupied())
    {
        return false;
    }

    Item* item = station->removeItem();

    return player->pickUpItem(item);
}

bool GameState::dropAtStation(int playerId, int stationId)
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

    if (!isPlayerAdjacentToStation(playerId, stationId))
    {
        return false;
    }

    if (!player->isHoldingItem())
    {
        return false;
    }

    Station* station = getStation(stationId);

    if (station == nullptr || station->isOccupied())
    {
        return false;
    }

    Item* item = player->dropItem();

    if (!station->placeItem(item))
    {
        player->pickUpItem(item);
        return false;
    }

    return true;
}

bool GameState::plateItem(int playerId, int stationId)
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

    if (!isPlayerAdjacentToStation(playerId, stationId))
    {
        return false;
    }

    if (!player->isHoldingItem())
    {
        return false;
    }

    Station* station = getStation(stationId);

    if (station == nullptr || !station->isOccupied())
    {
        return false;
    }

    Item* plate = station->getItem();

    if (plate == nullptr || plate->getType() != ItemType::Plate)
    {
        return false;
    }

    Item* heldItem = player->getHeldItem();

    if (!plate->addToPlate(heldItem->getType(),heldItem->getState()))
    {
        return false;
    }

    player->dropItem();

    return true;
}

RecipeType GameState::checkPlateRecipe(int stationId)
{
    Station* station = getStation(stationId);

    if (station == nullptr || !station->isOccupied())
    {
        return RecipeType::None;
    }

    Item* item = station->getItem();

    if (item == nullptr || item->getType() != ItemType::Plate)
    {
        return RecipeType::None;
    }

    return item->getRecipeType();
}

int GameState::getScore() const
{
    return score;
}

bool GameState::serveOrder(int playerId, int stationId)
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

    if (station->getType() != StationType::ServingStation)
    {
        return false;
    }

    if (!station->isOccupied())
    {
        return false;
    }

    Item* plate = station->getItem();

    if (plate == nullptr || plate->getType() != ItemType::Plate)
    {
        return false;
    }

    RecipeType recipe = plate->getRecipeType();

    if (recipe == RecipeType::None)
    {
        return false;
    }

    for (Order& order : orders)
    {
        if (!order.isCompleted() &&
            !order.isExpired() &&
            order.getRecipe() == recipe)
        {
            if (recipe == RecipeType::Salad)
            {
                score += 100;
            }
            else if (recipe == RecipeType::Burger)
            {
                score += 150;
            }

            order.complete();
            station->removeItem();

            return true;
        }
    }

    return false;
}

void GameState::addOrder(RecipeType recipe, int timeLimit)
{
    int newId = static_cast<int>(orders.size()) + 1;

    orders.emplace_back(newId, recipe, timeLimit);
}

void GameState::updateOrders(int seconds)
{
    for (Order& order : orders)
    {
        order.updateTimer(seconds);
    }
}

const std::vector<Order>& GameState::getOrders() const
{
    return orders;
}

void GameState::updateCookingTimers(int seconds)
{
    for (Station& station : stations)
    {
        station.updateCookingTimer(seconds);
    }
}

bool GameState::handleAction(int playerId, ActionType action, int targetId)
{
    switch (action)
    {
        case ActionType::MoveUp:
            return movePlayer(playerId, 0, -1);

        case ActionType::MoveDown:
            return movePlayer(playerId, 0, 1);

        case ActionType::MoveLeft:
            return movePlayer(playerId, -1, 0);

        case ActionType::MoveRight:
            return movePlayer(playerId, 1, 0);

        case ActionType::Pickup:
            return pickupFromStation(playerId, targetId);

        case ActionType::Drop:
            return dropAtStation(playerId, targetId);

        case ActionType::Chop:
            return chopAtStation(playerId, targetId);

        case ActionType::Cook:
            return startCookingAtStation(playerId, targetId);

        case ActionType::Plate:
            return plateItem(playerId, targetId);

        case ActionType::Serve:
            return serveOrder(playerId, targetId);

        default:
            return false;
    }
}

GameSnapshot GameState::getSnapshot() const
{
    GameSnapshot snapshot{};

    // -----------------------------
    // Player 1
    // -----------------------------

    snapshot.player1.playerId = player1.getId();
    snapshot.player1.x = player1.getX();
    snapshot.player1.y = player1.getY();
    snapshot.player1.holdingItem = player1.isHoldingItem();

    if (player1.isHoldingItem())
    {
        snapshot.player1.heldItemType =
            player1.getHeldItem()->getType();
    }

    // -----------------------------
    // Player 2
    // -----------------------------

    snapshot.player2.playerId = player2.getId();
    snapshot.player2.x = player2.getX();
    snapshot.player2.y = player2.getY();
    snapshot.player2.holdingItem = player2.isHoldingItem();

    if (player2.isHoldingItem())
    {
        snapshot.player2.heldItemType =
            player2.getHeldItem()->getType();
    }

    // -----------------------------
    // Stations
    // -----------------------------

    for (int i = 0; i < 5; i++)
    {
        const Station& station = stations[i];

        snapshot.stations[i].stationId =
            station.getId();

        snapshot.stations[i].occupied =
            station.isOccupied();

        snapshot.stations[i].cookingTimeRemaining =
            station.getCookingTimeRemaining();

        if (station.isOccupied())
        {
            Item* item = station.getItem();

            snapshot.stations[i].itemType =
                item->getType();

            snapshot.stations[i].itemState =
                item->getState();
        }
    }

    // -----------------------------
    // Orders
    // -----------------------------

    for (int i = 0; i < 2; i++)
    {
        snapshot.orders[i].orderId =
            orders[i].getId();

        snapshot.orders[i].recipe =
            orders[i].getRecipe();

        snapshot.orders[i].timeRemaining =
            orders[i].getTimeRemaining();

        snapshot.orders[i].completed =
            orders[i].isCompleted();

        snapshot.orders[i].expired =
            orders[i].isExpired();
    }

    // -----------------------------
    // Score
    // -----------------------------

    snapshot.score = score;

    return snapshot;
}