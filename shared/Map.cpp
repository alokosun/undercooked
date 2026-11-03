#include "Map.h"
#include <iostream>

Map::Map()
{
    width = 10;
    height = 8;

    tiles = {
        {TileType::Wall, TileType::Wall, TileType::Wall, TileType::Wall, TileType::Wall, TileType::Wall, TileType::Wall, TileType::Wall, TileType::Wall, TileType::Wall},

        {TileType::Wall, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Wall},

        {TileType::Wall, TileType::Floor, TileType::IngredientStation, TileType::Counter, TileType::ChoppingStation, TileType::Floor, TileType::Stove, TileType::Counter, TileType::Floor, TileType::Wall},

        {TileType::Wall, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Wall},

        {TileType::Wall, TileType::Floor, TileType::Counter, TileType::Floor, TileType::Counter, TileType::Floor, TileType::PlateStation, TileType::ServingStation, TileType::Floor, TileType::Wall},

        {TileType::Wall, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Wall},

        {TileType::Wall, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Floor, TileType::Wall},

        {TileType::Wall, TileType::Wall, TileType::Wall, TileType::Wall, TileType::Wall, TileType::Wall, TileType::Wall, TileType::Wall, TileType::Wall, TileType::Wall}
    };
}

int Map::getWidth() const
{
    return width;
}

int Map::getHeight() const
{
    return height;
}

TileType Map::getTile(int x, int y) const
{
    return tiles[y][x];
}

bool Map::isWalkable(int x, int y) const
{
    if (x < 0 || x >= width || y < 0 || y >= height)
    {
        return false;
    }

    return tiles[y][x] == TileType::Floor;
}

void Map::printMap() const
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            switch (tiles[y][x])
            {
                case TileType::Floor:
                    std::cout << ". ";
                    break;

                case TileType::Wall:
                    std::cout << "# ";
                    break;

                case TileType::Counter:
                    std::cout << "C ";
                    break;

                case TileType::IngredientStation:
                    std::cout << "I ";
                    break;

                case TileType::ChoppingStation:
                    std::cout << "H ";
                    break;

                case TileType::Stove:
                    std::cout << "S ";
                    break;

                case TileType::PlateStation:
                    std::cout << "P ";
                    break;

                case TileType::ServingStation:
                    std::cout << "V ";
                    break;
            }
        }

        std::cout << std::endl;
    }
}