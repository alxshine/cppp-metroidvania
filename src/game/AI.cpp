#include "game/AI.hpp"

using namespace game;
void IdleAI::controlEntity(Movable &movable, Attackable &, const CollisionMap &, Rectangle)
{
	movable.stopMoving();
}
void StandingAI::controlEntity(Movable &movable, Attackable &attackable, const CollisionMap &, Rectangle playerHitbox)
{
	movable.stopMoving();
	// find attacks in range
	for (unsigned i = 0; i < attackable.attacks.size(); ++i) {
		auto attackHitbox = attackable.getHitbox(movable.getPosition(), i, movable.getDirection().x < 0);
		if (intersects(attackHitbox, playerHitbox)) {
			attackable.attack(i);
			std::cout << "using attack " << i << std::endl;
			break;
		} else {
			std::cout << "attack not in range" << std::endl;
		}
	}
}
