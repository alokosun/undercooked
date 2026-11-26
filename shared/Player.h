#ifndef PLAYER_H
#define PLAYER_H
#include "Item.h"

class Player {
private:
    int id;
    float x;
    float y;
    Item* heldItem;

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
};

#endif