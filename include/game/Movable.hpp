#ifndef MOVABLE_H
#define MOVABLE_H

#include "constants.hpp"
#include "movement.hpp"

namespace game {

class Player;
struct Room;

class Movable {
  private:
	Position position;
	Position lastPosition; // Probably needed for collision detection...
	Direction direction = {0, 0};
	bool moved = false;
	bool canMove = true;

	Speed maxSpeed;

	friend class Game;
	friend void resolvePlayerCollision(Player &player, Room &currentRoom);

  public:
	Movable(Speed maxSpeed, Position pos = {0, 0});
	Velocity v;
	bool grounded = false;

	void update();
	void move(std::chrono::milliseconds frameDelta);
	void reposition(Position newPosition);

	bool getMoved() const;
	Direction getDirection() const;
	void setDirection(Direction d);
	Position getPosition() const;
	Position getLastPosition() const;
};
} // namespace game

#endif /* MOVABLE_H */
