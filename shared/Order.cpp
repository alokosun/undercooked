#include "Order.h"

Order::Order(int orderId, RecipeType recipeType, int timeLimit)
    : id(orderId),
      recipe(recipeType),
      timeRemaining(timeLimit),
      completed(false)
{
}

int Order::getId() const
{
    return id;
}

RecipeType Order::getRecipe() const
{
    return recipe;
}

int Order::getTimeRemaining() const
{
    return timeRemaining;
}

bool Order::isCompleted() const
{
    return completed;
}

bool Order::isExpired() const
{
    return timeRemaining <= 0 && !completed;
}

void Order::updateTimer(int seconds)
{
    if (completed)
    {
        return;
    }

    timeRemaining -= seconds;

    if (timeRemaining < 0)
    {
        timeRemaining = 0;
    }
}

void Order::complete()
{
    completed = true;
}