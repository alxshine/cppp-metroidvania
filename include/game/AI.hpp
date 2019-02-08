#ifndef _AI_H_
#define _AI_H_

#include <chrono>

#include "game/Attackable.hpp"
#include "game/Movable.hpp"
#include "game/constants.hpp"
#include "game/physics.hpp"

namespace game {
struct AI {
	virtual void controlEntity(Movable &movable, Attackable &attackable, const CollisionMap &collisionMap,
	                           Rectangle playerHitbox) = 0;
	virtual ~AI() noexcept = default;
};

struct IdleAI : AI {
	virtual void controlEntity(Movable &movable, Attackable &attackable, const CollisionMap &collisionMap,
	                           Rectangle playerHitbox) override;
};

struct StandingAI : AI {
	virtual void controlEntity(Movable &movable, Attackable &attackable, const CollisionMap &collisionMap,
	                           Rectangle playerHitbox) override;
};

struct ConfinedPatrollingAI : AI {
	virtual void controlEntity(Movable &movable, Attackable &attackable, const CollisionMap &collisionMap,
	                           Rectangle playerHitbox) override;
};

struct PatrollingAI : AI {
	virtual void controlEntity(Movable &movable, Attackable &attackable, const CollisionMap &collisionMap,
	                           Rectangle playerHitbox) override;
};

struct TwoPhaseBossAI : AI {
	virtual void controlEntity(Movable &movable, Attackable &attackable, const CollisionMap &collisionMap,
	                           Rectangle playerHitbox) override;
};
} // namespace game

#endif /* _AI_H_ */
