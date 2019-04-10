cmake_minimum_required(VERSION 3.13)
project(game_proj)

set(CMAKE_CXX_STANDARD 11)

include_directories(/home/andreas/Desktop/libtcod-1.5.1/include/)
link_directories(/home/andreas/Desktop/libtcod-1.5.1)

add_executable(game_proj main.cpp main.h)

target_link_libraries(game_proj -ltcod -ltcodxx)