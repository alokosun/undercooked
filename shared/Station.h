#ifndef STATION_H
#define STATION_H

#include "Item.h"

enum class StationType
{
    IngredientStation,
    ChoppingStation,
    Stove,
    PlateStation,
    ServingStation
};

class Station
{
private:
    int id;
    StationType type;
    int x;
    int y;
    Item* currentItem;

public:
    Station(int stationId, StationType stationType, int xPos, int yPos);

    int getId() const;
    StationType getType() const;
    int getX() const;
    int getY() const;

    bool isOccupied() const;

    Item* getItem() const;

    bool placeItem(Item* item);
    Item* removeItem();

    bool chopItem();
    bool startCooking();
    bool finishCooking();
};

#endif