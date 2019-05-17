cmake_minimum_required(VERSION 3.13)
project(Connector)

set(CMAKE_CXX_STANDARD 14)

include_directories(/home/andreas/CLionProjects/Connector/cmake-build-debug/)
include_directories(/home/andreas/cmake-3.6.2/Utilities/cmcurl/include/)
include_directories(/home/andreas/Desktop/xlnt-master/include/)


link_directories(/home/andreas/cmake-3.6.2/Utilities/cmcurl/)
link_directories(/home/andreas/Desktop/libxl-3.8.5.0/)
link_directories(/home/andreas/Desktop/xlnt-master)

add_executable(Connector main.cpp)

target_link_libraries(Connector PRIVATE xlnt -lcurl)
