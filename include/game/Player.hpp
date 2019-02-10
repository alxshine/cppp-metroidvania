#ifndef PLAYER_H
#define PLAYER_H

#include <algorithm>
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
	void shadeBlast();
	void singleSlam();
	void doubleSlam();
	void updateCombat(sdl::GameClock::duration frameDelta);
	Rectangle getAttackHitbox();

	Movable movable;
	Attackable attackable;
	std::set<std::string> visitedRooms = {"First"};

	Rectangle calcPositionedHitbox() const;
	Rectangle calcLastPositionedHitbox() const;

	std::set<Item> inventory;

	void addXp(int xp);
	void setLeveling(int level, int xp);
	inline int getLevel() const
	{
		return level;
	}
	inline int getXp() const
	{
		return xp;
	}
	inline float getDamageMultiplier() const
	{
		return 0.5 + 0.5 * level;
	}
	const int levelThreshold = 100;

  private:
	static constexpr Rectangle hitbox{0, 0, tileSize.w, tileSize.h * 2};
	Rectangle calcRenderTarget() const;
	sdl::Animation idleAnimation;
	sdl::Animation slideAnimation;
	bool isSliding = false;
	int comboCount = 0;
	int level = 1;
	int xp = 0;
	const sdl::GameClock::duration comboTimer = sdl::GameClock::duration(50);

	inline bool hasControl()
	{
		return !isSliding && movable.canMove && !attackable.isHurting() && !attackable.isAttacking();
	}
};
} // namespace game

#endif /* PLAYER_H */
