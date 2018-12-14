#include "game/Movable.hpp"

using namespace game;

Movable::Movable(Speed maxSpeed, Position pos) : position(pos), lastPosition(pos), maxSpeed(maxSpeed) {}

void Movable::move(std::chrono::milliseconds frameDelta)
{
	if(!canMove)
		return;
	lastPosition = position;
	position += v * frameDelta;
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
	int x = 0;
	if(v.x > 0_ups)
		x = 1;
	else if(v.x < 0_ups)
		x = -1;

	int y = 0;
	if(v.y > 0_ups)
		y = 1;
	else if(v.y < 0_ups)
		y = -1;

	return {x,y};
}

void Movable::setDirection(Direction d){
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
