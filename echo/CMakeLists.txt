cmake_minimum_required(VERSION 3.13)
project(echo)

set(CMAKE_CXX_STANDARD 11)

include_directories(/home/andreas/Desktop/libtcod-1.5.1/include/)
link_directories(/home/andreas/Desktop/libtcod-1.5.1)

add_executable(echo server.cpp)

target_link_libraries(echo -ltcod -ltcodxx)