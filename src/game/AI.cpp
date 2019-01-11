#include "game/AI.hpp"

using namespace game;
void IdleAI::controlEntity(Movable &movable, Attackable &, const CollisionMap &, Rectangle)
{
	movable.stopMoving();
}
