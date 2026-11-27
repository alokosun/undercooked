#include <iostream>
#include <iomanip>

#include "packetmaker.h"
#include "protocol.h"
#include "serialization.h"
#include "api.h"
#include "GameState.h"
#include "Item.h"
#include "protocol.h"
#include "string_helpers.cpp"

short BUFFER_CAPACITY = 80;


int main() {
    std::cout << "Buffer Capacity: " << BUFFER_CAPACITY << std::endl;
    uint8_t BUFFER[BUFFER_CAPACITY];
    Buffer_Writer w = Buffer_Writer(BUFFER, BUFFER_CAPACITY);
    Buffer_Reader r = Buffer_Reader(BUFFER, BUFFER_CAPACITY);

    GameSnapshot test;

    std::cout << "Player Test: sending player 67 with position (-67, -67) holding a tomato" << std::endl;
    test.player1 = PlayerSnapshot {67, -67, -67, true, ItemType::Tomato};
    test.player2 = PlayerSnapshot {1, 69, 69, true, ItemType::Plate};
    test.stations[0] = StationSnapshot {0, true, ItemType::Bun, ItemState::Raw, 0};
    test.stations[1] = StationSnapshot {1, true, ItemType::Patty, ItemState::Cooking, 2};
    test.stations[2] = StationSnapshot {2, true, ItemType::Lettuce, ItemState::Chopped, 0};
    test.stations[3] = StationSnapshot {3, false};
    test.stations[4] = StationSnapshot {4, true, ItemType::Patty, ItemState::Cooked, 1000};
    test.orders[0] = OrderSnapshot {0, RecipeType::Burger, 30, false, false};
    test.orders[1] = OrderSnapshot {1, RecipeType::Salad, 30, true, false};
    test.score = int('s') + int('t') + int('i') + int('n') + int('k') + int('y');

    std::cout << "Test: " << std::endl << to_string(test) << std::endl;

    encode(w, to_struct(test));
    std::cout << "After encode, " << std::to_string(w.remaining()) << " bytes remaining." << std::endl;

    std::cout << "BUFFER" << std::endl;
    for (int i = 0; i < BUFFER_CAPACITY; i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(BUFFER[i]) << " ";
        if (i % 8 == 7) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;

    UDP_Snapshot api_fail;
    decode(r, api_fail);
    GameSnapshot test_decode = to_class(api_fail);
    std::cout << "DECODED TEST" << std::endl << to_string(test_decode) << std::endl;

}

void clear_buffer(uint8_t* b, size_t capacity) {
    for (int i = 0; i < capacity; i++) {
        b[i] = 0;
    }
}


