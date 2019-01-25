#include "game/Room.hpp"

game::Rectangle game::Room::Tile::render(const sdl::Renderer &renderer, const game::Position targetPosition) const
{
	game::Rectangle targetRect{targetPosition.x, targetPosition.y, tileSize.w, tileSize.h};
	renderer.render(sprite, targetRect);
	return targetRect;
}

game::Room::Room(std::string name, const sdl::Texture &background, const sdl::Music &music, Position location,
                 int gatingArea, game::Room::Layout layout, game::CollisionMap collisionMap,
                 std::vector<game::Mob> mobs, std::vector<game::Item> items, std::vector<game::Item> onClearItems,
                 std::vector<game::Door> doors)
    : name(name), background(background), music(music), location(location), gatingArea(gatingArea), layout(layout),
      sizeInPixels({0, 0, static_cast<int>(layout[0][0].size()) * game::tileSize.w,
                    static_cast<int>(layout[0].size()) * game::tileSize.h}),
      collisionMap(collisionMap), mobs(mobs), items(items), onClearItems(onClearItems), doors((doors))
{
}

game::Room::Room(const Room &rhs) noexcept
    : name(rhs.name), background(rhs.background), music(rhs.music), location(rhs.location), gatingArea(rhs.gatingArea),
      layout(rhs.layout), sizeInPixels(rhs.sizeInPixels), collisionMap(rhs.collisionMap), mobs(rhs.mobs),
      items(rhs.items), doors(rhs.doors)
{
}

void game::Room::render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
                        const sdl::RenderOptions &options)
{
	// TODO: parallax
	renderer.render(background);

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

	for (auto i : doors)
		renderer.render(i.item, frameDelta, options);
	for (auto &i : mobs)
		renderer.render(i, frameDelta, options);
	for (auto &i : items)
		if (!i.pickedUp) // TODO remove from room?
			renderer.render(i, frameDelta, options);
}

game::Room::~Room() {}
