#include "game/Movable.hpp"

using namespace game;

Movable::Movable(Speed maxSpeed, Position pos) : position(pos), lastPosition(pos), maxSpeed(maxSpeed), v({0,0}) {}

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

void Movable::moveLeft()
{
	v.x -= maxSpeed;
}

void Movable::moveRight()
{
	v.x += maxSpeed;
}

void Movable::jump()
{
	if (grounded) {
		v.y = -2 * maxSpeed;
		grounded = false;
	}
}

void Movable::fall()
{
	fallThroughPlatforms = true;
	grounded = false;
	v.y = 2 * maxSpeed;
}

void Movable::applyGravity(std::chrono::milliseconds frameDelta)
{
	if (!grounded) {
		v.y += 600 * frameDelta.count() / 1000; // the value for gravity was found via trial-and-error
    v.y = std::min(v.y, 2*maxSpeed);
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
