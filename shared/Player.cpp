#include "Player.h"

Player::Player(int playerId, float startX, float startY)
    : id(playerId),
      x(startX),
      y(startY),
      heldItem(nullptr),
      facing(Direction::Down),
      currentAction(PlayerAction::Idle)
{
}

int Player::getId() const
{
    return id;
}

float Player::getX() const
{
    return x;
}

float Player::getY() const
{
    return y;
}

void Player::setPosition(float newX, float newY)
{
    x = newX;
    y = newY;
}

void Player::move(float dx, float dy)
{
    x += dx;
    y += dy;
}

bool Player::isHoldingItem() const
{
    return heldItem != nullptr;
}

Item* Player::getHeldItem() const
{
    return heldItem;
}

bool Player::pickUpItem(Item* item)
{
    if (heldItem != nullptr || item == nullptr)
    {
        return false;
    }

    heldItem = item;
    return true;
}

Item* Player::dropItem()
{
    Item* item = heldItem;
    heldItem = nullptr;

    return item;
}

Direction Player::getFacing() const
{
    return facing;
}

void Player::setFacing(Direction direction)
{
    facing = direction;
}

PlayerAction Player::getCurrentAction() const
{
    return currentAction;
}

void Player::setCurrentAction(PlayerAction action)
{
    currentAction = action;
}