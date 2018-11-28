#ifndef PLAYER_H
#define PLAYER_H

#include "Movable.hpp"
#include "SDL.hpp"
#include "constants.hpp"

namespace game {
class Player : public sdl::Renderable {
  public:
	Player(const sdl::Animation idleAnimation, const sdl::Animation walkingAnimation);
	// virtual ~Player();

	Player &operator=(const Player &rhs) = delete;
	Player(const Player &rhs) = delete;

	void render(const sdl::Renderer &renderer, const sdl::GameClock::time_point &t,
	            const sdl::RenderOptions &options = {}) const override;

	Movable movable;

  private:
	const sdl::Animation idleAnimation;
	const sdl::Animation walkingAnimation;
};
} // namespace game

#endif /* PLAYER_H */
