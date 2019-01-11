#ifndef ROOM_H
#define ROOM_H

#include <algorithm>
#include <vector>

#include "Item.hpp"
#include "Mob.hpp"
#include "SDL.hpp"
#include "constants.hpp"

namespace game {

struct Room : public sdl::Renderable {
	struct Tile {
		const sdl::Sprite sprite;
		Rectangle render(const sdl::Renderer &renderer, const Position targetPosition) const;
	};

	using Row = std::vector<Tile>;
	using Layer = std::vector<Row>;
	using Layout = std::vector<Layer>;

	Room(const std::string name, const sdl::Texture &background, const sdl::Music &music, const Position &location,
	     const Layout layout, const CollisionMap collisionMap, const std::vector<Mob> mobs,
	     const std::vector<Item> items, const std::vector<Door> doors);
	Room(const Room &rhs) noexcept;
	virtual ~Room();

	void render(const sdl::Renderer &renderer, sdl::GameClock::duration,
	            const sdl::RenderOptions &options = {}) override;

	const std::string name;
	const sdl::Texture &background;
	const sdl::Music &music;
	const Position location;
	const Layout layout;
	const Rectangle sizeInPixels;
	const CollisionMap collisionMap;
	std::vector<Mob> mobs;
	std::vector<Item> items;
	std::vector<Door> doors;
};

inline Rectangle tileRectangle(int row, int column)
{
	return {column * tileSize.w, row * tileSize.h, tileSize.w, tileSize.h};
}

} // namespace game

#endif /* ROOM_H */
