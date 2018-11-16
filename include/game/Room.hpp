#ifndef ROOM_H
#define ROOM_H

#include <vector>

#include "SDL.hpp"
#include "constants.hpp"

namespace game {

struct Room : public sdl::Renderable {
  private:
	struct Tile {
		const sdl::Sprite *sprite;
		void render(const sdl::Renderer &renderer, const Rectangle targetLocation) const;
	};

  public:
	Room(const std::string name, const sdl::Texture &background, const sdl::Music &music, const Position &location,
	     const std::vector<std::vector<Tile>> layout);
	Room(const Room &rhs);                             // TODO set default state in copy ctor
	Room &operator=(const Room &rhs) = delete;
	virtual ~Room();

	void render(const sdl::Renderer &renderer, const sdl::GameClock::time_point &) const override;

	const std::string name;
	const sdl::Texture &background;
	const sdl::Music &music;
	const Position location;
	const std::vector<std::vector<Tile>> layout;
};

} // namespace game

#endif /* ROOM_H */
