#include "game/Movable.hpp"

using namespace game;

Movable::Movable(int speedPerSecond, Position pos) : position(pos), lastPosition(pos), speedPerSecond(speedPerSecond) {}

void Movable::move(Position delta, std::chrono::milliseconds frameDelta)
{
	lastPosition = position;
	position += delta * static_cast<int>(speedPerSecond * frameDelta.count() / 1000);

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
