#ifndef MOVABLE_H
#define MOVABLE_H

#include "constants.hpp"
#include "movement.hpp"

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

	friend class Game;
	friend void resolveRoomCollision(Entity &player, Room &currentRoom);

  public:
	Movable(Speed maxSpeed, Position pos = {0, 0});
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
};
} // namespace game

#endif /* MOVABLE_H */
