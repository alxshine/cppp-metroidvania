#ifndef MOVABLE_H
#define MOVABLE_H

#include "GameClock.hpp"
#include "constants.hpp"
#include "movement.hpp"

namespace game {

class Player;
struct Room;

class Movable {
  private:
	Position position;
	Position lastPosition; // needed for collision detection...
	Direction direction = {0, 0};
	bool moved = false;
	bool isMoving = false;
	bool grounded = false;
	int maxJumps;
	int jumps = maxJumps;
	inline void setGrounded()
	{ // also a separate function because this will reset the jump count
		jumps = maxJumps;
		grounded = true;
	}

	Speed maxSpeed;
	OptionalAnimation runningAnimation;
	OptionalAnimation airUpAnimation;
	OptionalAnimation airDownAnimation;

	inline Rectangle hitboxHelper(Position pos) const
	{
		return {pos.x - hitbox.w / 2, pos.y - hitbox.h, hitbox.w, hitbox.h};
	}

	friend class Game;
	friend void resolveRoomCollision(Movable &movable, const CollisionMap &collisionMap);

  public:
	Movable(Rectangle hitbox, Speed maxSpeed, sdl::Animation runningAnimation, sdl::Animation airUpAnimation,
	        sdl::Animation airDownAnimation, Position pos = {0, 0}, int maxJumps = 0);
	Movable(Rectangle hitbox, Speed maxSpeed, sdl::Animation runningAnimation, Position pos = {0, 0}, int maxJumps = 0);
	Movable(Rectangle hitbox, Speed maxSpeed, Position pos = {0, 0}, int maxJumps = 0);
	Movable(const Movable &rhs);
	Movable &operator=(const Movable &rhs);
	Rectangle hitbox;
	Velocity v;
	bool canMove = true;
	bool fallThroughPlatforms = false;
	Position initialPosition;

	Rectangle calcPositionedHitbox() const;
	Rectangle calcLastPositionedHitbox() const;

	void mainLoopReset();
	void moveLeft();
	void moveRight();
	void jump();
	void fall();
	void applyGravity(std::chrono::milliseconds frameDelta);

	void update(std::chrono::milliseconds frameDelta);
	void reposition(Position newPosition);

	bool getMoved() const;
	bool getIsMoving() const;
	Direction getDirection() const;
	void setDirection(Direction d);
	Position getPosition() const;
	Position getLastPosition() const;

	void startMoving();
	void stopMoving();
	bool hasPlayableAnimation() const;
	sdl::Sprite updateAnimation(sdl::GameClock::duration frameDelta);
};
} // namespace game

#endif /* MOVABLE_H */
