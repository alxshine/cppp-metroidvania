#ifndef MOVABLE_H
#define MOVABLE_H

#include "constants.hpp"
#include "movement.hpp"

namespace game {

class Movable {
  private:
	Position position;
	Position lastPosition; // Probably needed for collision detection...
	Direction direction = Direction::Right;
	bool moved = false;
	bool canMove = true;

	Speed maxSpeed;

	friend class Game;

  public:
	Movable(Speed maxSpeed, Position pos = {0, 0});
	Velocity v;
	bool grounded;

	void update();
	void move(std::chrono::milliseconds frameDelta);
	void reposition(Position newPosition);

	bool getMoved() const;
	Direction getDirection() const;
	void setDirection(Direction dir);
	Position getPosition() const;
	Position getLastPosition() const;
};
} // namespace game

#endif /* MOVABLE_H */
