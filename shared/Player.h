#ifndef PLAYER_H
#define PLAYER_H

#include "Item.h"

enum class Direction
{
    Up,
    Down,
    Left,
    Right
};

enum class PlayerAction
{
    Idle,
    PickingUp,
    Dropping,
    Chopping,
    Cooking,
    Plating,
    Serving
};

class Player
{
private:
    int id;
    float x;
    float y;

    Item* heldItem;

    Direction facing;
    PlayerAction currentAction;

public:
    Player(int playerId, float startX, float startY);

    int getId() const;

    float getX() const;
    float getY() const;

    void setPosition(float newX, float newY);
    void move(float dx, float dy);

    bool isHoldingItem() const;
    Item* getHeldItem() const;

    bool pickUpItem(Item* item);
    Item* dropItem();

    Direction getFacing() const;
    void setFacing(Direction direction);

    PlayerAction getCurrentAction() const;
    void setCurrentAction(PlayerAction action);
};

#endif