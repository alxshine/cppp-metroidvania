#ifndef ROOM_H
#define ROOM_H

#include <vector>

#include "SDL.hpp"
#include "constants.hpp"

namespace game {

struct Room {
  private:
	struct Tile : public sdl::Renderable {
		const sdl::Sprite *sprite;
		void render(const sdl::Renderer &renderer, const sdl::GameClock::time_point &t) const override;
	};

  public:
	Room(const std::string name, const sdl::Texture& background, const sdl::Music &music, const Position& location, const std::vector<std::vector<Tile>> layout);                // TODO ctor
	Room(const Room &rhs); // TODO set default state in copy ctor
	Room &operator=(const Room &rhs) = delete;
	virtual ~Room();

	const std::string name;
	const sdl::Texture &background;
	const sdl::Music &music;
	const Position location;
	const std::vector<std::vector<Tile>> layout;
};

} // namespace game

#endif /* ROOM_H */
