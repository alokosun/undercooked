#include <iostream>
#include "Player.h"

int main() {
    std::cout << "Undercooked Server Starting..." << std::endl;

    Player player1(1, 2.0f, 3.0f);

    std::cout << "Player " << player1.getId()
              << " spawned at ("
              << player1.getX()
              << ", "
              << player1.getY()
              << ")"
              << std::endl;

    return 0;
}