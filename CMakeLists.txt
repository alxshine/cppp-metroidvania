cmake_minimum_required(VERSION 2.6)
project(metroidvania)

set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(default_build_type "Release")
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

INCLUDE(FindPkgConfig)
###############################################

add_subdirectory(src)

add_subdirectory(lib)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY game_definitions DESTINATION ${CMAKE_BINARY_DIR})

# game
add_executable(game main.cpp)
target_link_libraries(game gamelib)
