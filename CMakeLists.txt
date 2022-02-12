cmake_minimum_required(VERSION 3.20)
project(Chip8)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)

include_directories(Chip8 ${SDL2_INCLUDE_DIRS})

add_executable(Chip8 src/main.cpp src/Chip8.cpp src/Utils.cpp src/Utils.h src/Chip8.h)
target_link_libraries(Chip8 ${SDL2_LIBRARIES})
