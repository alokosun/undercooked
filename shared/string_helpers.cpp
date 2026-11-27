#ifndef STRING_HELPERS_CPP
#define STRING_HELPERS_CPP

/*
Mildly batshit number of to_string functions. For debugging usage.
*/

#include "api.h"
#include "Item.h"
#include "protocol.h"

std::string to_string(ItemType i) {
    switch (i) {
        case ItemType::Lettuce:
            return "Lettuce";
        case ItemType::Tomato:
            return "Tomato";
        case ItemType::Bun:
            return "Bun";
        case ItemType::Patty:
            return "Patty";
        case ItemType::Plate:
            return "Plate";
    }
    return "Unknown";
}

std::string to_string(ItemState i) {
    switch (i) {
        case ItemState::Raw:
            return "Raw";
        case ItemState::Chopped:
            return "Chopped";
        case ItemState::Cooking:
            return "Cooking";
        case ItemState::Cooked:
            return "Cooked";
    }
    return "Unknown";
}

std::string to_string(RecipeType r) {
    switch (r) {
        case RecipeType::None:
            return "None";
        case RecipeType::Salad:
            return "Salad";
        case RecipeType::Burger:
            return "Burger";
    }
    return "Unknown";
}


std::string to_string(const PlayerSnapshot& s) {
    std::string str = "Player " + std::to_string(s.playerId) + " at (" + std::to_string(s.x) + "," + std::to_string(s.y) + ") holding ";
    if (!s.holdingItem) return str + "nothing";
    else return str + to_string(s.heldItemType);
}

std::string to_string(const StationSnapshot& s) {
    std::string str = ("Station " + std::to_string(s.stationId) + " ");
    if (!s.occupied) return str + "unoccupied";
    else {
        str += "with " + to_string(s.itemState) + " " + to_string(s.itemType);
        if (s.itemState == ItemState::Cooking) str += " with " + std::to_string(s.cookingTimeRemaining) + " seconds remaining.";
    }
    return str;
}

std::string to_string(const OrderSnapshot& s) {
    std::string str;
    if (s.completed) str += "Completed ";
    if (s.expired) str += "Expired ";
    str += to_string(s.recipe) + " order " + std::to_string(s.orderId) + " with " + 
        std::to_string(s.timeRemaining) + " seconds remaining";
    return str;
}

std::string to_string(const GameSnapshot& s) {
    std::string str = to_string(s.player1) + "\n" + to_string(s.player2) + "\n";
    for (int i = 0; i < 5; i++) {
        str += to_string(s.stations[i]) + "\n";
    }
    for (int i = 0; i < 2; i++) {
        str += to_string(s.orders[i]) + "\n";
    }
    return str + "Score: " + std::to_string(s.score) + " points";
}

#endif