#ifndef PLAYER_H
#define PLAYER_H

#include <chrono>
#include <set>
#include <vector>

#include "Attackable.hpp"
#include "Item.hpp"
#include "Movable.hpp"
#include "SDL.hpp"
#include "constants.hpp"

namespace game {
class Player : public sdl::Renderable {
  public:
	Player(sdl::Animation idleAnimation, sdl::Animation walkingAnimation, sdl::Animation airUpAnimation,
	       sdl::Animation airDownAnimation, sdl::Animation deathAnimation, sdl::Animation hurtAnimation,
	       sdl::Animation slideAnimation, std::vector<Attack> attacks);

	Player &operator=(const Player &rhs) = delete;
	Player(const Player &rhs) = delete;

	void render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
	            const sdl::RenderOptions &options = {}) override;

	void startMoving();
	void stopMoving();

	void moveLeft();
	void moveRight();
	void jump();
	void fall();
	void slide();

	void update(sdl::GameClock::duration gameFrameDelta);

	void attack();
	void updateCombat(sdl::GameClock::duration frameDelta);
	Rectangle getAttackHitbox();

	Movable movable;
	Attackable attackable;
	std::set<std::string> visitedRooms = {"First"};

	Rectangle calcPositionedHitbox() const;
	Rectangle calcLastPositionedHitbox() const;

	std::set<Item> inventory;

  private:
	static constexpr Rectangle hitbox{0, 0, tileSize.w, tileSize.h * 2};
	Rectangle calcRenderTarget() const;
	sdl::Animation idleAnimation;
	sdl::Animation slideAnimation;
	bool isSliding = false;
	int comboCount = 0;
	const sdl::GameClock::duration comboTimer = sdl::GameClock::duration(50);

	inline bool hasControl()
	{
		return !isSliding && movable.canMove && !attackable.isHurting() && !attackable.isAttacking();
	}
};
} // namespace game

#endif /* PLAYER_H */
