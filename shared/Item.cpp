#include "Item.h"

Item::Item(int itemId, ItemType itemType)
    : id(itemId), type(itemType), state(ItemState::Raw)
{
}

int Item::getId() const
{
    return id;
}

ItemType Item::getType() const
{
    return type;
}

ItemState Item::getState() const
{
    return state;
}

void Item::setState(ItemState newState)
{
    state = newState;
}

bool Item::canChop() const
{
    return state == ItemState::Raw;
}

bool Item::canCook() const
{
    return type == ItemType::Patty &&
           state == ItemState::Chopped;
}