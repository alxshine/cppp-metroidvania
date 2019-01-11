#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

#include "Movable.hpp"
#include "Attackable.hpp"
#include "SDL.hpp"
#include "constants.hpp"

namespace game {
class Player : public sdl::Renderable {
  public:
	Player(const sdl::Animation idleAnimation, const sdl::Animation walkingAnimation, const sdl::Animation airUpAnimation, const sdl::Animation airDownAnimation, const std::vector<Attack> attacks);

	Player &operator=(const Player &rhs) = delete;
	Player(const Player &rhs) = delete;

	void render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
	            const sdl::RenderOptions &options = {}) override;

  void startMoving();
  void stopMoving();
  
  void attack();
  void updateCombat(sdl::GameClock::duration frameDelta);
  Rectangle getAttackHitbox();

	static constexpr Rectangle hitbox{static_cast<int>(tileSize.w / 2), 0, tileSize.w, tileSize.h * 2};
	Movable movable;
  Attackable attackable;

	Rectangle calcPositionedHitbox() const;
	Rectangle calcLastPositionedHitbox() const;

  private:
	Rectangle calcRenderTarget() const;
	sdl::Animation idleAnimation;
  int comboCount = 0;
  sdl::GameClock::duration timeSinceLastAttack = sdl::GameClock::duration::zero();
  const sdl::GameClock::duration comboTimer = sdl::GameClock::duration(50);
};
} // namespace game

#endif /* PLAYER_H */
