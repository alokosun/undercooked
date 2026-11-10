cmake_minimum_required(VERSION 3.16)

project(Undercooked)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(undercooked_server
    server/main.cpp
    shared/Player.cpp
    shared/Map.cpp
    shared/GameState.cpp
    shared/Item.cpp
    shared/Station.cpp
)

target_include_directories(undercooked_server PRIVATE shared)