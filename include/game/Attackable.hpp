#ifndef _ATTACKABLE_H_
#define _ATTACKABLE_H_

#include <unordered_set>
#include <vector>

#include "GameClock.hpp"
#include "game/Movable.hpp"
#include "game/constants.hpp"

namespace game {
class Attackable {
  public:
	int maxHp;
	int hp;
  int poise;

	Attackable(int maxHp, int poise, const std::vector<Attack> attacks, sdl::Animation deathAnimation,
	           sdl::Animation hurtAnimation);
	std::vector<Attack> attacks;
	inline bool isDead()
	{
		return hp == 0;
	};
	void attack(int attackIndex);

	Rectangle getHitbox(Position position, bool flip = false);
	Rectangle getHitbox(Position position, int attackIndex, bool flip = false);
	void hit(Attackable &other);
	void getHit(int damage);
	void update(sdl::GameClock::duration frameDelta);
	inline bool isAttacking() const
	{
		return currentAttack >= 0;
	}

	inline sdl::Sprite getCurrentSprite()
	{
		return attacks[currentAttack].animation.getCurrentFrame();
	}

	inline bool done()
	{
		return hp <= 0 && deathAnimation.getLoopCount() > 0;
	}
  
	bool hasPlayableAnimation() const;
  sdl::Sprite updateAnimation(sdl::GameClock::duration frameDelta);

  private:
	int currentAttack = -1;
	sdl::GameClock::duration currentAttackTime;
	std::unordered_set<Attackable *> alreadyHit;
	sdl::Animation deathAnimation;
	sdl::Animation hurtAnimation;
};
} // namespace game

#endif /* _ATTACKABLE_H_ */
