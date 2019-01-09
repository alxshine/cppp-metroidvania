#ifndef PLAYER_H
#define PLAYER_H

#include "Movable.hpp"
#include "SDL.hpp"
#include "constants.hpp"

namespace game {
class Player : public sdl::Renderable {
  public:
	Player(const sdl::Animation idleAnimation, const sdl::Animation walkingAnimation, const sdl::Animation airUpAnimation, const sdl::Animation airDownAnimation);

	Player &operator=(const Player &rhs) = delete;
	Player(const Player &rhs) = delete;

	void render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
	            const sdl::RenderOptions &options = {}) override;

	static constexpr Rectangle hitbox{static_cast<int>(tileSize.w / 2), 0, tileSize.w, tileSize.h * 2};
	Movable movable;

	Rectangle calcPositionedHitbox() const;
	Rectangle calcLastPositionedHitbox() const;

  private:
	Rectangle calcRenderTarget() const;
	sdl::Animation idleAnimation;
};
} // namespace game

#endif /* PLAYER_H */
