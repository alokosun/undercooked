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

bool Item::addToPlate(ItemType itemType, ItemState itemState)
{
    if (type != ItemType::Plate)
    {
        return false;
    }

    contents.push_back({itemType, itemState});
    return true;
}

const std::vector<PlateIngredient>& Item::getContents() const
{
    return contents;
}

RecipeType Item::getRecipeType() const
{
    if (type != ItemType::Plate)
    {
        return RecipeType::None;
    }

    bool choppedLettuce = false;
    bool choppedTomato = false;
    bool bun = false;
    bool cookedPatty = false;

    for (const PlateIngredient& ingredient : contents)
    {
        if (ingredient.type == ItemType::Lettuce &&
            ingredient.state == ItemState::Chopped)
        {
            choppedLettuce = true;
        }

        else if (ingredient.type == ItemType::Tomato &&
                 ingredient.state == ItemState::Chopped)
        {
            choppedTomato = true;
        }

        else if (ingredient.type == ItemType::Bun)
        {
            bun = true;
        }

        else if (ingredient.type == ItemType::Patty &&
                 ingredient.state == ItemState::Cooked)
        {
            cookedPatty = true;
        }
    }

    if (choppedLettuce && choppedTomato &&
        !bun && !cookedPatty)
    {
        return RecipeType::Salad;
    }

    if (bun && cookedPatty &&
        !choppedLettuce && !choppedTomato)
    {
        return RecipeType::Burger;
    }

    return RecipeType::None;
}