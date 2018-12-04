#include "game/Room.hpp"

game::Rectangle game::Room::Tile::render(const sdl::Renderer &renderer, const game::Position targetPosition) const
{
	game::Rectangle targetRect{targetPosition.x, targetPosition.y, tileSize.w, tileSize.h};
	renderer.render(sprite, targetRect);
	return targetRect;
}

game::Room::Room(const std::string name, const sdl::Texture &background, const sdl::Music &music,
                 const Position &location, const game::Room::Layout layout, const game::Room::CollisionMap collisionMap,
                 const std::vector<game::Mob> mobs, const std::vector<game::Item> items)
    : name(name), background(background), music(music), location(location), layout(layout), collisionMap(collisionMap),
      mobs(mobs), items(items)
{
}

game::Room::Room(const Room &rhs) noexcept
    : name(rhs.name), background(rhs.background), music(rhs.music), location(rhs.location), layout(rhs.layout),
      collisionMap(rhs.collisionMap), mobs(rhs.mobs), items(rhs.items)
{
}

void game::Room::render(const sdl::Renderer &renderer, const sdl::GameClock::time_point &t,
                        const sdl::RenderOptions &options) const
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

	if (options.renderCollisionMap) {
		auto locationY = 0;
		for (auto &row : collisionMap) {
			auto locationX = 0;
			for (auto &tile : row) {
				game::Rectangle targetRect{locationX, locationY, game::tileSize.w, game::tileSize.h};
				sdl::Color color{0, 0, 0, 100};
				switch (tile) {
				case Collision::TopOnly:
					color.g = 255;
					break;
				case Collision::Full:
					color.r = 255;
					break;
				case Collision::None:
					break;
				}
				renderer.drawRectangle(targetRect, color);
				locationX += tileSize.w;
			}
			locationY += tileSize.h;
		}
	}

	for (auto i : mobs)
		renderer.render(i, t, options);
	for (auto i : items)
		renderer.render(i, t, options);
}

game::Room::~Room() {}
