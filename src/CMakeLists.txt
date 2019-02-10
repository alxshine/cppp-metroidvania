add_library(gamelib
    gamedef/definition_loading.cpp
    # game/Mixins.cpp
    menu/InventoryMenu.cpp
    menu/MessageBox.cpp
    menu/SelectionMenu.cpp
    menu/MapMenu.cpp
    menu/StatMenu.cpp
    state/SerializedState.cpp
    game/Game.cpp
    game/Item.cpp
    game/Mob.cpp
    game/Movable.cpp
    game/Attackable.cpp
    game/AI.cpp
    game/Player.cpp
    game/ResourceManager.cpp
    game/Room.cpp
    game/movement.cpp
    game/attacks.cpp
    game/room_helpers.cpp
)
target_include_directories(gamelib PUBLIC ../include)
target_link_libraries(gamelib stdc++fs)
target_link_libraries(gamelib SDL_Wrapper)

add_executable(parsertest parsertest.cpp)
target_link_libraries(parsertest gamelib)
