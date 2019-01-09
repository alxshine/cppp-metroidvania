#ifndef MOVABLE_H
#define MOVABLE_H

#include "constants.hpp"
#include "movement.hpp"
#include "GameClock.hpp"

namespace game {

class Entity;
struct Room;

class Movable {
  private:
	Position position;
	Position lastPosition; // needed for collision detection...
	Direction direction = {0, 0};
  bool moved = false;
	bool grounded = true;

	Speed maxSpeed;
  const OptionalAnimation runningAnimation;
  const OptionalAnimation airUpAnimation;
  const OptionalAnimation airDownAnimation;

	friend class Game;
	friend void resolveRoomCollision(Entity &player, Room &currentRoom);

  public:
	Movable(Speed maxSpeed, sdl::Animation runningAnimation, sdl::Animation airUpAnimation, sdl::Animation airDownAnimation, Position pos = {0, 0});
  Movable(Speed maxSpeed, sdl::Animation runningAnimation, Position pos = {0,0});
  Movable(Speed maxSpeed, Position pos = {0,0});
  Movable(const Movable& rhs);
	Velocity v;
	bool canMove = true;
	bool fallThroughPlatforms = false;

  void mainLoopReset();
  void moveLeft();
  void moveRight();
  void jump();
  void fall();
  void applyGravity(std::chrono::milliseconds frameDelta);

	void update(std::chrono::milliseconds frameDelta);
	void reposition(Position newPosition);

	bool getMoved() const;
	Direction getDirection() const;
	void setDirection(Direction d);
	Position getPosition() const;
	Position getLastPosition() const;

  bool hasPlayableAnimation() const;
  sdl::Sprite getAnimationFrame(sdl::GameClock::time_point t) const;
};
} // namespace game

#endif /* MOVABLE_H */
