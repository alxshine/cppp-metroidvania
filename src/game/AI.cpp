#include "game/AI.hpp"

using namespace game;

void attackIfHits(Movable &movable, Attackable &attackable, Rectangle playerHitbox)
{
	// find attacks in range
	for (unsigned i = 0; i < attackable.attacks.size(); ++i) {
		auto attackHitbox = attackable.getHitbox(movable.getPosition(), i, movable.getDirection().x < 0);
		if (intersects(attackHitbox, playerHitbox)) {
			attackable.attack(i);
			break;
		}
	}
}

void IdleAI::controlEntity(Movable &movable, Attackable &, const CollisionMap &, Rectangle)
{
	movable.stopMoving();
}
void StandingAI::controlEntity(Movable &movable, Attackable &attackable, const CollisionMap &, Rectangle playerHitbox)
{
	movable.stopMoving();
  attackIfHits(movable, attackable, playerHitbox);
}

void PatrollingAI::controlEntity(Movable &movable, Attackable &attackable, const CollisionMap &,
                                 Rectangle playerHitbox)
{
  const int patrolDistance = 2*tileSize.w; //just a benchmark
  auto xDiff = movable.getPosition().x - movable.initialPosition.x;
  if(xDiff > patrolDistance){
    //we are at the right end of the patrol area
    movable.moveLeft();
  }else if(xDiff < -patrolDistance){
    //left end
    movable.moveRight();
  }else{
    //otherwise continue moving
    if(movable.getDirection().x >= 0)
      movable.moveRight();
    else
      movable.moveLeft();
  }

  attackIfHits(movable, attackable, playerHitbox);
}
