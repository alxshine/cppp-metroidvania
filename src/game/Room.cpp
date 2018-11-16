#include "game/Room.hpp"
void game::Room::Tile::render(const sdl::Renderer &renderer, const game::Rectangle targetLocation) const
{
	if (sprite != nullptr)
		renderer.render(*sprite, targetLocation);
}

game::Room::Room(const std::string name, const sdl::Texture &background, const sdl::Music &music,
                 const Position &location, const std::vector<std::vector<Tile>> layout)
    : name(name), background(background), music(music), location(location), layout(layout)
{
}

game::Room::Room(const Room &rhs)
    : name(rhs.name), background(rhs.background), music(rhs.music), location(rhs.location), layout(rhs.layout)
{
}

void game::Room::render(const sdl::Renderer &renderer, const sdl::GameClock::time_point &) const
{
	renderer.render(background); // TODO tell renderer where to render background?

	// TODO render tiles

	// TODO render entities
}

game::Room::~Room() {}
