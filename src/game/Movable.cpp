#include "game/Movable.hpp"

using namespace game;

Movable::Movable(Speed maxSpeed, Position pos) : position(pos), lastPosition(pos), maxSpeed(maxSpeed) {}

void Movable::update(std::chrono::milliseconds frameDelta)
{
	if (!canMove)
		return;
	lastPosition = position;
	position += v * frameDelta;

	if (lastPosition != position) {
		direction.x = (v.x > 0) - (v.x < 0);
		direction.y = (v.y > 0) - (v.y < 0);
	}
}

void Movable::reposition(Position newPosition)
{
	lastPosition = position;
	position = newPosition;
}

bool Movable::getMoved() const
{
	return canMove && lastPosition != position;
}
Direction Movable::getDirection() const
{
	return direction;
}

void Movable::setDirection(Direction d)
{
	direction = d;
}

Position Movable::getPosition() const
{
	return position;
}
Position Movable::getLastPosition() const
{
	return lastPosition;
}
