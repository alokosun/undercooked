#include "Station.h"

Station::Station(int stationId, StationType stationType, int xPos, int yPos)
    : id(stationId),
      type(stationType),
      x(xPos),
      y(yPos),
      currentItem(nullptr),
      cookingTimeRemaining(0),
      cookingDuration(5)
{
}

int Station::getX() const
{
    return x;
}

int Station::getY() const
{
    return y;
}

int Station::getId() const
{
    return id;
}

StationType Station::getType() const
{
    return type;
}

bool Station::isOccupied() const
{
    return currentItem != nullptr;
}

Item* Station::getItem() const
{
    return currentItem;
}

bool Station::placeItem(Item* item)
{
    if (currentItem != nullptr || item == nullptr)
    {
        return false;
    }

    currentItem = item;
    return true;
}

Item* Station::removeItem()
{
    Item* item = currentItem;
    currentItem = nullptr;
    return item;
}

bool Station::chopItem()
{
    if (type != StationType::ChoppingStation ||
        currentItem == nullptr ||
        !currentItem->canChop())
    {
        return false;
    }

    currentItem->setState(ItemState::Chopped);
    return true;
}

bool Station::startCooking()
{
    if (type != StationType::Stove ||
        currentItem == nullptr ||
        !currentItem->canCook())
    {
        return false;
    }

    currentItem->setState(ItemState::Cooking);
    cookingTimeRemaining = cookingDuration;

    return true;
}

bool Station::finishCooking()
{
    if (type != StationType::Stove ||
        currentItem == nullptr ||
        currentItem->getState() != ItemState::Cooking)
    {
        return false;
    }

    currentItem->setState(ItemState::Cooked);
    return true;
}

void Station::updateCookingTimer(int seconds)
{
    if (!isCooking())
    {
        return;
    }

    cookingTimeRemaining -= seconds;

    if (cookingTimeRemaining <= 0)
    {
        cookingTimeRemaining = 0;
        currentItem->setState(ItemState::Cooked);
    }
}

int Station::getCookingTimeRemaining() const
{
    return cookingTimeRemaining;
}

bool Station::isCooking() const
{
    return currentItem != nullptr &&
           currentItem->getState() == ItemState::Cooking;
}