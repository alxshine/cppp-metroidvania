#include "game/Room.hpp"

game::Rectangle game::Room::Tile::render(const sdl::Renderer &renderer, const game::Position targetPosition) const
{
	sdl::Rectangle targetRect{targetPosition.x, targetPosition.y, tileSize.w, tileSize.h};
	renderer.render(sprite, targetRect);
	return targetRect;
}

game::Room::Room(const std::string name, const sdl::Texture &background, const sdl::Music &music,
                 const Position &location, const game::Room::Layout layout, const game::Room::CollisionMap collisionMap)
    : name(name), background(background), music(music), location(location), layout(layout), collisionMap(collisionMap)
{
}

game::Room::Room(const Room &rhs) noexcept
    : name(rhs.name), background(rhs.background), music(rhs.music), location(rhs.location), layout(rhs.layout), collisionMap(rhs.collisionMap)
{
}

void game::Room::render(const sdl::Renderer &renderer, const sdl::GameClock::time_point &) const
{
	renderer.render(background,
	                sdl::Rectangle{0, -10, sdl::Renderer::logicalW,
	                               sdl::Renderer::logicalH}); // TODO tell renderer where to render background?
	for (auto &layer : layout) {
		auto locationY = 0;
		for (auto &row : layer) {
			auto locationX = 0;
			for (auto &tile : row) {
				tile.render(renderer, {locationX, locationY});
				locationX += tileSize.w;
			}
			locationY += tileSize.h;
		}
	}

	// TODO render entities
}

game::Room::~Room() {}
