#ifndef MAP_H
#define MAP_H

#include <vector>
#include "TileType.h"

class Map
{
private:
    int width;
    int height;

    std::vector<std::vector<TileType>> tiles;

public:
    Map();

    int getWidth() const;
    int getHeight() const;

    TileType getTile(int x, int y) const;

    bool isWalkable(int x, int y) const;

    void printMap() const;
};

#endif