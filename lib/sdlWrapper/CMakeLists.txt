cmake_minimum_required (VERSION 2.6)
project(SDL_Wrapper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(default_build_type "Release")
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

include_directories(${SDL_Wrapper_SOURCE_DIR}/include)
add_library(SDL_Wrapper src/texture.cpp src/renderer.cpp src/SDL.cpp src/font.cpp src/gameClock.cpp src/animation.cpp)
target_link_libraries(SDL_Wrapper SDL2 SDL2_image SDL2_ttf)
