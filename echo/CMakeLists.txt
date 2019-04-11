cmake_minimum_required(VERSION 3.13)
project(server)

set(CMAKE_CXX_STANDARD 11)

include_directories(/home/andreas/CLionProjects/echo/cmake-build-debug/)
include_directories(/home/andreas/Desktop/libtcod-1.5.1/include/)

link_directories(/home/andreas/Desktop/libtcod-1.5.1)

add_executable(server server.cpp)
add_executable(Client Client.cpp)
add_executable(Client2 Client2.cpp)

target_link_libraries(server -ltcod -ltcodxx)
target_link_libraries(Client -ltcod -ltcodxx)
target_link_libraries(Client2 -ltcod -ltcodxx)