#include "game/AI.hpp"

using namespace game;
using namespace std::literals::chrono_literals;

std::vector<int> getHittingAttacks(const Movable &movable, const Attackable &attackable, Rectangle playerHitbox)
{
	std::vector<int> ret;

	// find attacks in range
	for (unsigned i = 0; i < attackable.attacks.size(); ++i) {
		auto attackHitbox = attackable.getHitbox(movable.getPosition(), i, movable.getDirection().x < 0);
		if (intersects(attackHitbox, playerHitbox)) {
			ret.push_back(i);
		}
	}

	return ret;
}

void IdleAI::controlEntity(Movable &movable, Attackable &, const CollisionMap &, Rectangle)
{
	movable.stopMoving();
}
void StandingAI::controlEntity(Movable &movable, Attackable &attackable, const CollisionMap &, Rectangle playerHitbox)
{
	movable.stopMoving();
	if (playerHitbox.x > movable.getPosition().x)
		movable.setDirection({1, 0});
	else
		movable.setDirection({-1, 0});

	if (attackable.getTimeSinceLastAttack() > 500ms) {
		auto hittingAttacks = getHittingAttacks(movable, attackable, playerHitbox);
		if (!hittingAttacks.empty())
			attackable.attack(hittingAttacks[0]);
	}
}

void PatrollingAI::controlEntity(Movable &movable, Attackable &attackable, const CollisionMap &, Rectangle playerHitbox)
{
	const int patrolDistance = 3 * tileSize.w; // just a benchmark

	if (attackable.isAttacking())
		return;

	auto hittingAttacks = getHittingAttacks(movable, attackable, playerHitbox);

	// Attack
	if (!hittingAttacks.empty()) {
		if (attackable.getTimeSinceLastAttack() > 100ms)
			attackable.attack(hittingAttacks[0]);
		return;
	}

	// Patrol
	auto xDiff = movable.getPosition().x - movable.initialPosition.x;
	if (xDiff > patrolDistance) {
		// we are at the right end of the patrol area
		movable.moveLeft();
	} else if (xDiff < -patrolDistance) {
		// left end
		movable.moveRight();
	} else {
		// otherwise continue moving
		if (movable.getDirection().x >= 0)
			movable.moveRight();
		else
			movable.moveLeft();
	}
}

void TwoPhaseBossAI::controlEntity(Movable &movable, Attackable &attackable, const CollisionMap &collisionMap,
                                   Rectangle playerHitbox)
{
	const int patrolDistance = 3 * tileSize.w; // just a benchmark

	if (attackable.isAttacking())
		return;

	// Attack
	auto hittingAttacks = getHittingAttacks(movable, attackable, playerHitbox);

	if (!hittingAttacks.empty()) {
		if (attackable.getTimeSinceLastAttack() > 1.5s) {
			if (attackable.hp >= attackable.maxHp / 2)
				attackable.attack(hittingAttacks[0]);
			else
				attackable.attack(hittingAttacks[1]);
		}
		return;
	}

	// Patrol
	auto xDiff = movable.getPosition().x - movable.initialPosition.x;
	if (xDiff > patrolDistance) {
		// we are at the right end of the patrol area
		movable.moveLeft();
	} else if (xDiff < -patrolDistance) {
		// left end
		movable.moveRight();
	} else {
		// otherwise continue moving
		if (movable.getDirection().x >= 0)
			movable.moveRight();
		else
			movable.moveLeft();
	}
}
