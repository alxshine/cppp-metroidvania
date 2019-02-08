#ifndef _ATTACKABLE_H_
#define _ATTACKABLE_H_

#include <unordered_set>
#include <vector>

#include "GameClock.hpp"
#include "game/Movable.hpp"
#include "game/attacks.hpp"

namespace game {
class Attackable {
  public:
	int maxHp;
	int hp;
	int poise;

	Attackable(int maxHp, int poise, std::vector<Attack> attacks, sdl::Animation deathAnimation,
	           sdl::Animation hurtAnimation,
	           sdl::GameClock::duration invulnerabilityWindow = sdl::GameClock::duration::zero());
	std::vector<Attack> attacks;
	inline bool isDead()
	{
		return hp == 0;
	};
	void attack(int attackIndex);

	Rectangle getHitbox(Position position, bool flip = false) const;
	Rectangle getHitbox(Position position, int attackIndex, bool flip = false) const;
	void hit(Attackable &other);
	void hurt(int damage);
	void update(sdl::GameClock::duration frameDelta);
	void updateProjectiles(sdl::GameClock::duration, Rectangle hitboxes, Attackable &attackable,
	                       const CollisionMap &collisionMap);
	void updateProjectiles(sdl::GameClock::duration, const std::vector<Rectangle> &hitboxes,
	                       std::vector<Attackable *> &attackables, const CollisionMap &collisionMap);
	void launchProjectiles(Position currentPosition, Direction currentDirection);
	inline bool isAttacking() const
	{
		return currentAttack >= 0;
	}
	inline bool isHurting() const
	{
		return hurting;
	}

	inline sdl::Sprite getCurrentSprite()
	{
		return attacks[currentAttack].animation.getCurrentFrame();
	}

	inline bool done()
	{
		return hp <= 0 && deathAnimation.getLoopCount() > 0;
	}

	inline void setInvulnerable()
	{
		vulnerable = false;
		vulnerabilityOverride = true;
	}

	inline void resetInvulnerable()
	{
		vulnerable = true;
		vulnerabilityOverride = false;
	}

	std::vector<Projectile> projectiles{};

	bool hasPlayableAnimation() const;
	void reset();
	sdl::Sprite updateAnimation(sdl::GameClock::duration frameDelta);

	inline sdl::GameClock::duration getTimeSinceLastAttack()
	{
		return timeSinceLastAttack;
	}

  private:
	int currentAttack = -1;
	bool vulnerable = true;
	bool vulnerabilityOverride = false;
	bool hurting = false;
	bool dealsDamage = false;
	sdl::GameClock::duration currentAttackTime;
	std::unordered_set<Attackable *> alreadyHit;
	sdl::Animation deathAnimation;
	sdl::Animation hurtAnimation;
	sdl::GameClock::duration invulnerabilityWindow;
	sdl::GameClock::duration lastHitTime = sdl::GameClock::duration::zero();
	sdl::GameClock::duration timeSinceLastAttack = sdl::GameClock::duration::zero();
};
} // namespace game

#endif /* _ATTACKABLE_H_ */
