#include "Player.h"

Player::Player(int playerId, float startX, float startY)
    : id(playerId), x(startX), y(startY) {
}

int Player::getId() const {
    return id;
}

float Player::getX() const {
    return x;
}

float Player::getY() const {
    return y;
}

void Player::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

void Player::move(float dx, float dy) {
    x += dx;
    y += dy;
}