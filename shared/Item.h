#ifndef ITEM_H
#define ITEM_H
#include <vector>

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

enum class RecipeType
{
    None,
    Salad,
    Burger
};

struct PlateIngredient
{
    ItemType type;
    ItemState state;
};

class Item
{
private:
    int id;
    ItemType type;
    ItemState state;
    std::vector<PlateIngredient> contents;

public:
    Item(int itemId, ItemType itemType);

    int getId() const;
    ItemType getType() const;
    ItemState getState() const;

    void setState(ItemState newState);

    bool canChop() const;
    bool canCook() const;

    RecipeType getRecipeType() const;

    bool addToPlate(ItemType itemType, ItemState itemState);
    const std::vector<PlateIngredient>& getContents() const;
};

#endif