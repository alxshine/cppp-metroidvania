#include "game/Movable.hpp"

using namespace game;

Movable::Movable(Speed maxSpeed, Position pos) : position(pos), lastPosition(pos), maxSpeed(maxSpeed) {}

void Movable::move(std::chrono::milliseconds frameDelta)
{
	if(!canMove)
		return;
	lastPosition = position;
	position += v * frameDelta;

	if (position.x > lastPosition.x)
		direction = Direction::Right;
	else if (position.x < lastPosition.x)
		direction = Direction::Left;

	// NOTE: this reflects top-down coordinates
	else if (position.y > lastPosition.y)
		direction = Direction::Down;
	else if (position.y < lastPosition.y)
		direction = Direction::Up;
}

void Movable::setDirection(Direction dir)
{
	direction = dir;
}

void Movable::reposition(Position newPosition) {
	position = newPosition;
	lastPosition = newPosition;
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
