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

	Room(std::string name, const sdl::Texture &background, const sdl::Music &music, Position location, int gatingArea,
	     Layout layout, CollisionMap collisionMap, std::vector<Mob> mobs, std::vector<Item> items,
	     std::vector<Door> doors);
	Room(const Room &rhs) noexcept;
	virtual ~Room();

	void render(const sdl::Renderer &renderer, sdl::GameClock::duration,
	            const sdl::RenderOptions &options = {}) override;

	const std::string name;
	const sdl::Texture &background;
	const sdl::Music &music;
	const Position location;
	const int gatingArea;
	const Layout layout;
	const Rectangle sizeInPixels;
	const CollisionMap collisionMap;
	std::vector<Mob> mobs;
	std::vector<Item> items;
	std::vector<Door> doors;
};

} // namespace game

#endif /* ROOM_H */
