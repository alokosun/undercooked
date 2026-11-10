#ifndef ITEM_H
#define ITEM_H

enum class ItemType
{
    Lettuce,
    Tomato,
    Bun,
    Patty,
    Plate
};

enum class ItemState
{
    Raw,
    Chopped,
    Cooking,
    Cooked
};

class Item
{
private:
    int id;
    ItemType type;
    ItemState state;

public:
    Item(int itemId, ItemType itemType);

    int getId() const;
    ItemType getType() const;
    ItemState getState() const;

    void setState(ItemState newState);

    bool canChop() const;
    bool canCook() const;
};

#endif