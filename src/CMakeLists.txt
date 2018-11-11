add_library(gamelib
    gamedef/DefinitionLoading.cpp
    # game/Mixins.cpp
    # game/Mob.cpp
    game/ResourceManager.cpp
    game/Mob.cpp
    game/Room.cpp
    game/Item.cpp
)
target_include_directories(gamelib PUBLIC ../include)
target_link_libraries(gamelib stdc++fs)
target_link_libraries(gamelib SDL_Wrapper)

add_executable(parsertest parsertest.cpp)
target_link_libraries(parsertest gamelib)
