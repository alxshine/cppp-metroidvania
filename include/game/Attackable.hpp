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
	const int maxHp;
	int hp;

	Attackable(int maxHp, const std::vector<Attack> attacks, Movable &movable);
	std::vector<Attack> attacks;
	int getSuitableAttackIndex(CollisionMap &collisionMap, Rectangle targetHitbox, Position currentPosition);
	inline bool isDead()
	{
		return hp == 0;
	};
	void attack(int attackIndex);

	Rectangle getHitbox(Position position, bool flip = false);
	void hit(Attackable &other);
	void update(sdl::GameClock::duration frameDelta);
	inline bool isAttacking()
	{
		return currentAttack >= 0;
	}

	inline sdl::Sprite getCurrentSprite()
	{
		return attacks[currentAttack].animation.getCurrentFrame();
	}

  private:
	Movable &movable;
	int currentAttack = -1;
	sdl::GameClock::duration currentAttackTime;
	std::unordered_set<Attackable *> alreadyHit;
};
} // namespace game

#endif /* _ATTACKABLE_H_ */