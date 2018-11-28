add_library(gamelib
    gamedef/definition_loading.cpp
    # game/Mixins.cpp
    game/Item.cpp
    game/Mob.cpp
    game/Movable.cpp
    game/Player.cpp
    game/ResourceManager.cpp
    game/Room.cpp
)
target_include_directories(gamelib PUBLIC ../include)
target_link_libraries(gamelib stdc++fs)
target_link_libraries(gamelib SDL_Wrapper)

add_executable(parsertest parsertest.cpp)
target_link_libraries(parsertest gamelib)
