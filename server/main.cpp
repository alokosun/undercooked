#include <iostream>
#include "GameState.h"

int main()
{
    GameState game;

    std::cout << "Undercooked Server Starting...\n\n";

    game.getMap().printMap();

    Player& player1 = game.getPlayer1();

    std::cout << "\nPlayer 1 starts at ("
              << player1.getX() << ", "
              << player1.getY() << ")\n";

    if (game.movePlayer(1, 1, 0))
    {
        std::cout << "Player 1 moved successfully.\n";
    }
    else
    {
        std::cout << "Player 1 movement blocked.\n";
    }

    std::cout << "Player 1 is now at ("
              << player1.getX() << ", "
              << player1.getY() << ")\n";

    return 0;
}