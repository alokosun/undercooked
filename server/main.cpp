#include <iostream>
#include "GameState.h"
#include "Item.h"

int main()
{
    GameState game;

    std::cout << "Undercooked Full Snapshot Test\n\n";

    Player& player1 = game.getPlayer1();

    // ---------------------------------
    // Movement
    // ---------------------------------

    game.handleAction(
        1,
        ActionType::MoveRight
    );

    // ---------------------------------
    // Put chopped patty on stove
    // ---------------------------------

    Item patty(1, ItemType::Patty);
    patty.setState(ItemState::Chopped);

    game.placeItemAtStation(
        &patty,
        3
    );

    // Move beside stove and start cooking
    player1.setPosition(5, 2);

    game.handleAction(
        1,
        ActionType::Cook,
        3
    );

    // Advance cooking timer by 2 seconds
    game.updateCookingTimers(2);

    // Advance order timers by 10 seconds
    game.updateOrders(10);

    // ---------------------------------
    // Get snapshot
    // ---------------------------------

    GameSnapshot snapshot =
        game.getSnapshot();

    // ---------------------------------
    // Player state
    // ---------------------------------

    std::cout << "=== Players ===\n";

    std::cout << "Player 1: ID="
              << snapshot.player1.playerId
              << " Position=("
              << snapshot.player1.x
              << ", "
              << snapshot.player1.y
              << ")\n";

    std::cout << "Player 2: ID="
              << snapshot.player2.playerId
              << " Position=("
              << snapshot.player2.x
              << ", "
              << snapshot.player2.y
              << ")\n\n";

    // ---------------------------------
    // Station state
    // ---------------------------------

    std::cout << "=== Stations ===\n";

    for (int i = 0; i < 5; i++)
    {
        std::cout << "Station "
                  << snapshot.stations[i].stationId
                  << ": ";

        if (snapshot.stations[i].occupied)
        {
            std::cout << "Occupied"
                      << " | Cooking Timer="
                      << snapshot.stations[i].cookingTimeRemaining;
        }
        else
        {
            std::cout << "Empty";
        }

        std::cout << "\n";
    }

    std::cout << "\n";

    // ---------------------------------
    // Order state
    // ---------------------------------

    std::cout << "=== Orders ===\n";

    for (int i = 0; i < 2; i++)
    {
        std::cout << "Order "
                  << snapshot.orders[i].orderId
                  << ": Time="
                  << snapshot.orders[i].timeRemaining
                  << " Completed="
                  << (snapshot.orders[i].completed
                          ? "YES"
                          : "NO")
                  << " Expired="
                  << (snapshot.orders[i].expired
                          ? "YES"
                          : "NO")
                  << "\n";
    }

    std::cout << "\n";

    // ---------------------------------
    // Score
    // ---------------------------------

    std::cout << "=== Score ===\n";

    std::cout << "Score: "
              << snapshot.score
              << "\n";

    return 0;
}