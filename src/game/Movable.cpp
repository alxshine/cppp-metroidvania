#include "game/Movable.hpp"

using namespace game;

Movable::Movable(Position pos) : position(pos), lastPosition(pos) {}

void Movable::move(Position newPosition)
{
	lastPosition = position;
	position = newPosition;

	if (newPosition.x > lastPosition.x)
		direction = Direction::Right;
	else if (newPosition.x < lastPosition.x)
		direction = Direction::Left;

	// NOTE: this reflects top-down coordinates
	else if (newPosition.y > lastPosition.y)
		direction = Direction::Down;
	else if (newPosition.y < lastPosition.y)
		direction = Direction::Up;
}

void Movable::update()
{
	if (position != lastPosition)
		moved = true;
	else
		moved = false;

	lastPosition = position;
}

bool Movable::getMoved() const
{
	return moved;
}
Direction Movable::getDirection() const
{
	return direction;
}
Position Movable::getPosition() const
{
	return position;
}
Position Movable::getLastPosition() const
{
	return lastPosition;
}
