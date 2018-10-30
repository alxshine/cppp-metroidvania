cmake_minimum_required(VERSION 2.6)
project(metroidvania)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra")
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
###############################################

# add_library(lib src)
add_subdirectory(lib)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# game
add_executable(game game.cpp)
target_include_directories(game PRIVATE lib/sdlWrapper/include)
target_link_libraries(game SDL_Wrapper)
