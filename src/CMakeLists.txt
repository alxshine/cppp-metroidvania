add_library(gamelib
    gamedef/DefinitionLoading.cpp
    gamedef/LoadAllDefinitions.cpp
    # game/Mixins.cpp
    # game/Mob.cpp
)
target_include_directories(gamelib PUBLIC ../include)
target_link_libraries(gamelib stdc++fs)

add_executable(parsertest parsertest.cpp)
target_link_libraries(parsertest gamelib)
