#include "game/Attackable.hpp"

using namespace game;

Attackable::Attackable(int maxHp, const std::vector<Attack> attacks, Movable &movable)
    : maxHp(maxHp), hp(maxHp), attacks(attacks), movable(movable)
{
}

void Attackable::attack(int attackIndex)
{
	if (isAttacking() || !movable.canMove)
		return;
	currentAttack = attackIndex;
	currentAttackTime = sdl::GameClock::duration::zero();
	attacks[currentAttack].animation.reset();
  alreadyHit.clear();
	movable.canMove = false;
};

Rectangle Attackable::getHitbox(Position position, bool flip){
  return getHitbox(position, currentAttack, flip);
}

Rectangle Attackable::getHitbox(Position position, int attackIndex, bool flip)
{
	if (attackIndex < 0)
		return {0, 0, 0, 0};

	auto currentHitbox = attacks[attackIndex].hitBox;
	if (flip)
		return {position.x - currentHitbox.x - currentHitbox.w, position.y + currentHitbox.y, currentHitbox.w,
		        currentHitbox.h};
	else
		return {position.x + currentHitbox.x, position.y + currentHitbox.y, currentHitbox.w, currentHitbox.h};
}
void Attackable::hit(Attackable &other)
{
	if (isAttacking() && alreadyHit.find(&other) == alreadyHit.end() ) {
		other.hp -= attacks[currentAttack].damage;
    alreadyHit.insert(&other);
	}
}
void Attackable::update(sdl::GameClock::duration frameDelta)
{
	if (!isAttacking())
		return;
	currentAttackTime += frameDelta;
	attacks[currentAttack].animation.updateAnimation(frameDelta);
	if (currentAttackTime > attacks[currentAttack].animation.totalDuration()) {
		currentAttack = -1;
		movable.canMove = true;
	}
}
