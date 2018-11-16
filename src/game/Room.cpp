#include "game/Room.hpp"
game::Room::Room(const std::string name, const sdl::Texture &background, const sdl::Music &music,
                 const Position &location, const std::vector<std::vector<Tile>> layout)
    : name(name), background(background), music(music), location(location), layout(layout)
{
}

game::Room::Room(const Room &rhs)
    : name(rhs.name), background(rhs.background), music(rhs.music), location(rhs.location), layout(rhs.layout)
{
}

game::Room::~Room() {}
