#include "game/Attackable.hpp"

using namespace game;

Attackable::Attackable(int maxHp, int poise, std::vector<Attack> attacks, sdl::Animation deathAnimation,
                       sdl::Animation hurtAnimation, sdl::GameClock::duration invulnerabilityWindow)
    : maxHp(maxHp), hp(maxHp), poise(poise), attacks(std::move(attacks)), deathAnimation(std::move(deathAnimation)),
      hurtAnimation(std::move(hurtAnimation)), invulnerabilityWindow(invulnerabilityWindow)
{
}

void Attackable::attack(int attackIndex)
{
	if (isAttacking() || (hurting && hurtAnimation.getLoopCount() == 0))
		return;
	currentAttack = attackIndex;
	currentAttackTime = sdl::GameClock::duration::zero();
	attacks[currentAttack].animation.reset();
	alreadyHit.clear();
};

Rectangle Attackable::getHitbox(Position position, bool flip)
{
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
	if (isAttacking() && alreadyHit.find(&other) == alreadyHit.end() && dealsDamage) {
		other.hurt(attacks[currentAttack].damage);
		alreadyHit.insert(&other);
	}
}

void Attackable::hurt(int damage)
{
	if (hp <= 0 || lastHitTime < invulnerabilityWindow)
		return;

	hp -= damage;
	if (damage > poise) {
		currentAttack = -1;
		hurtAnimation.reset();
		hurting = true;
		lastHitTime = sdl::GameClock::duration::zero();
	}
}

bool Attackable::hasPlayableAnimation() const
{
	return isAttacking() || (hp <= 0 && deathAnimation.getLoopCount() == 0) ||
	       (hurting && hurtAnimation.getLoopCount() == 0);
}

sdl::Sprite Attackable::updateAnimation(sdl::GameClock::duration frameDelta)
{
	if (currentAttack >= 0)
		return attacks[currentAttack].animation.updateAnimation(frameDelta);
	if (hp <= 0)
		return deathAnimation.updateAnimation(frameDelta);
	else
		return hurtAnimation.updateAnimation(frameDelta);
}

void Attackable::reset()
{
	hp = maxHp;
	deathAnimation.reset();
}

void Attackable::update(sdl::GameClock::duration frameDelta)
{
	lastHitTime += frameDelta;

	if (hurting && hurtAnimation.getLoopCount() > 0)
		hurting = false;

	if (isAttacking()) {
		auto &attack = attacks[currentAttack];

		// update currentAttackTime
		int oldIndex = currentAttackTime / attack.animation.getTimePerFrame();
		currentAttackTime += frameDelta;
		int currentIndex = currentAttackTime / attack.animation.getTimePerFrame();

		// check if we have just passed a damage frame -> we deal damage currently
		dealsDamage = false;
		for (auto i : attack.damageFrames)
			if (oldIndex < i && currentIndex >= i)
				dealsDamage = true;

		// check if the current attack is done
		if (currentAttackTime > attack.animation.totalDuration())
			currentAttack = -1;
	}
}

void Attackable::updateProjectiles(sdl::GameClock::duration gameFrameDelta, Rectangle playerHitbox, Attackable &other,
                                   const CollisionMap &collisionMap)
{
	for (auto &p : projectiles)
		if (p.update(gameFrameDelta, playerHitbox, collisionMap))
			other.hurt(p.damage);

	projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(), [](auto p) { return p.done; }),
	                  projectiles.end());
}

void Attackable::launchProjectiles(Position currentPosition, Direction currentDirection)
{
	if (!isAttacking() || !dealsDamage || attacks[currentAttack].type != Attack::Type::Ranged)
		return;

	// spawn a projectile
	auto &bluePrint = attacks[currentAttack].projectileBlueprint;
	auto startPosition = currentPosition + bluePrint->startPosition;
	Velocity v{bluePrint->maxSpeed, 0};
	if (currentDirection.x < 0)
		v.x = -v.x;
	projectiles.emplace_back(startPosition, bluePrint->hitBox, bluePrint->damage, bluePrint->noClip, v,
	                         bluePrint->animation);
}
