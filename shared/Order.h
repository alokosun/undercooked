#ifndef ORDER_H
#define ORDER_H

#include "Item.h"

class Order
{
private:
    int id;
    RecipeType recipe;
    int timeRemaining;
    bool completed;

public:
    Order(int orderId, RecipeType recipeType, int timeLimit);

    int getId() const;
    RecipeType getRecipe() const;
    int getTimeRemaining() const;

    bool isCompleted() const;
    bool isExpired() const;

    void updateTimer(int seconds);
    void complete();
};

#endif