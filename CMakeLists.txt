cmake_minimum_required(VERSION 2.6)
project(metroidvania)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(default_build_type "RelWithDebugInfo")
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

INCLUDE(FindPkgConfig)
###############################################

add_subdirectory(src)

add_subdirectory(lib)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
file(COPY game_definitions DESTINATION ${CMAKE_BINARY_DIR})

# game
add_executable(game game.cpp)
target_include_directories(game PRIVATE lib/sdlWrapper/include)
target_link_libraries(game SDL_Wrapper)
