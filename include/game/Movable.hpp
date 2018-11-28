#ifndef MOVABLE_H
#define MOVABLE_H

#include "constants.hpp"

namespace game {

class Movable {
  private:
	Position position;
	Position lastPosition; // Probably needed for collision detection...
	Direction direction = Direction::Right;
	bool moved = false;

	int speedPerSecond;

  public:
	Movable(int speedPerSecond, Position pos = {0, 0});

	void update();
	void move(Position delta, std::chrono::milliseconds frameDelta);
	void reposition(Position newPosition);

	bool getMoved() const;
	Direction getDirection() const;
	Position getPosition() const;
	Position getLastPosition() const;
};
} // namespace game

#endif /* MOVABLE_H */
