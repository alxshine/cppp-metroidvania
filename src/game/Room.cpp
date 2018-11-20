#include "game/Room.hpp"
game::Rectangle game::Room::Tile::render(const sdl::Renderer &renderer, const game::Position targetPosition) const
{
	sdl::Rectangle targetRect{targetPosition.x, targetPosition.y, tileSize.w, tileSize.h};
	renderer.render(sprite, targetRect);
	return targetRect;
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
	auto locationY = 0;
	for (auto &row : layout) {
		auto locationX = 0;
		for (auto &tile : row) {
			tile.render(renderer, {locationX, locationY});
			locationX += tileSize.w;
		}
		locationY += tileSize.h;
	}

	// TODO render entities
}

game::Room::~Room() {}
