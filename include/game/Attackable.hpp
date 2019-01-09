#ifndef _ATTACKABLE_H_
#define _ATTACKABLE_H_

#include <vector>

#include "GameClock.hpp"
#include "game/Room.hpp"
#include "game/constants.hpp"

namespace game {
template <typename T>
class Attackable {
  public:
	unsigned maxHp;
	unsigned hp;

	Attackable(unsigned maxHp, const std::vector<Attack> attacks, T &owner)
	    : maxHp(maxHp), hp(maxHp), attacks(attacks), owner(owner)
	{
	}

	std::vector<Attack> attacks;
	int getSuitableAttackIndex(Room::CollisionMap &collisionMap, Rectangle targetHitbox, Position currentPosition);
	inline bool isDead()
	{
		return hp == 0;
	};
	void attack(int attackIndex)
	{
		if (currentAttack >= 0)
			return;
		currentAttack = attackIndex;
		currentAttackTime = sdl::GameClock::duration::zero();
    attacks[currentAttack].animation.reset();
		owner.movable.canMove = false;
	};

	void update(sdl::GameClock::duration frameDelta)
	{
		if (!isAttacking())
			return;
		currentAttackTime += frameDelta;
    attacks[currentAttack].animation.updateAnimation(frameDelta);
		if (currentAttackTime > attacks[currentAttack].animation.totalDuration()) {
			currentAttack = -1;
			owner.movable.canMove = true;
		}
	}

	inline bool isAttacking()
	{
		return currentAttack >= 0;
	}

	sdl::Sprite getCurrentSprite()
	{
		return attacks[currentAttack].animation.getCurrentFrame();
	}

  private:
	T &owner;
	int currentAttack = -1;
	sdl::GameClock::duration currentAttackTime;
};
} // namespace game

#endif /* _ATTACKABLE_H_ */
