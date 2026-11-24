#include <iostream>
#include "GameState.h"
#include "Item.h"

int main()
{
    std::cout << "Undercooked Interaction Test\n\n";

    GameState game;

    // Chopping station is at (4,2)
    Item lettuce(1, ItemType::Lettuce);

    // Put Player 1 next to the chopping station
    game.getPlayer1().setPosition(3.0f, 2.0f);

    std::cout << "Player 1 position: ("
              << game.getPlayer1().getX() << ", "
              << game.getPlayer1().getY() << ")\n";

    std::cout << "Testing adjacent interaction...\n";

    game.placeItemAtStation(&lettuce, 2);

    if (game.chopAtStation(1, 2))
    {
        std::cout << "PASS: Player 1 was close enough and chopped the lettuce.\n";
    }
    else
    {
        std::cout << "FAIL: Player 1 should have been able to chop.\n";
    }

    std::cout << "\nMoving Player 1 far away...\n";

    // Move player far away
    game.getPlayer1().setPosition(1.0f, 1.0f);

    std::cout << "Player 1 position: ("
              << game.getPlayer1().getX() << ", "
              << game.getPlayer1().getY() << ")\n";

    // Reset lettuce so it can be chopped again
    lettuce.setState(ItemState::Raw);

    std::cout << "Testing distant interaction...\n";

    if (game.chopAtStation(1, 2))
    {
        std::cout << "FAIL: Player 1 should NOT be able to chop from far away.\n";
    }
    else
    {
        std::cout << "PASS: Interaction correctly rejected because Player 1 is too far away.\n";
    }

    return 0;
}