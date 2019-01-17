#include "game/Movable.hpp"

using namespace game;

Movable::Movable(Rectangle hitbox, Speed maxSpeed, sdl::Animation runningAnimation, sdl::Animation airUpAnimation,
                 sdl::Animation airDownAnimation, Position pos)
    : position(pos), lastPosition(pos), maxSpeed(maxSpeed),
      runningAnimation(std::make_unique<sdl::Animation>(runningAnimation)),
      airUpAnimation(std::make_unique<sdl::Animation>(airUpAnimation)),
      airDownAnimation(std::make_unique<sdl::Animation>(airDownAnimation)), hitbox(hitbox),
      v({0, 0}), initialPosition{pos}
{
}

Movable::Movable(Rectangle hitbox, Speed maxSpeed, sdl::Animation runningAnimation, Position pos)
    : position(pos), lastPosition(pos), maxSpeed(maxSpeed),
      runningAnimation(std::make_unique<sdl::Animation>(runningAnimation)), airUpAnimation(nullptr),
      airDownAnimation(nullptr), hitbox(hitbox), v({0, 0}), initialPosition{pos}
{
}

Movable::Movable(Rectangle hitbox, Speed maxSpeed, Position pos)
    : position(pos), lastPosition(pos), maxSpeed(maxSpeed), runningAnimation(nullptr), airUpAnimation(nullptr),
      airDownAnimation(nullptr), hitbox(hitbox), initialPosition{pos}
{
}

Movable::Movable(const Movable &rhs)
    : position(rhs.position), lastPosition(rhs.lastPosition), maxSpeed(rhs.maxSpeed),
      runningAnimation(rhs.runningAnimation != nullptr ? std::make_unique<sdl::Animation>(*rhs.runningAnimation)
                                                       : nullptr),
      airUpAnimation(rhs.airUpAnimation != nullptr ? std::make_unique<sdl::Animation>(*rhs.airUpAnimation) : nullptr),
      airDownAnimation(rhs.airDownAnimation != nullptr ? std::make_unique<sdl::Animation>(*rhs.airDownAnimation)
                                                       : nullptr),
      hitbox(rhs.hitbox), v({0, 0}), initialPosition{rhs.initialPosition}
{
}

game::Rectangle Movable::calcPositionedHitbox() const{
  return {position.x - hitbox.w/2, position.y - hitbox.h, hitbox.w, hitbox.h};
}

void Movable::update(sdl::GameClock::duration frameDelta)
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

void Movable::mainLoopReset()
{
	v.x = 0;
	fallThroughPlatforms = false;
	moved = false;
}

void Movable::moveLeft()
{
	v.x -= maxSpeed;
	moved = true;
}

void Movable::moveRight()
{
	v.x += maxSpeed;
	moved = true;
}

void Movable::jump()
{
	if (grounded) {
		v.y = -2 * maxSpeed;
		grounded = false;
		moved = true;
	}
}

void Movable::fall()
{
	fallThroughPlatforms = true;
	grounded = false;
	v.y = 2 * maxSpeed;
	moved = true;
}

void Movable::applyGravity(std::chrono::milliseconds frameDelta)
{
	if (!grounded) {
		v.y += 600 * frameDelta.count() / 1000; // the value for gravity was found via trial-and-error
		v.y = std::min(v.y, 2 * maxSpeed);
	}
}

void Movable::reposition(Position newPosition)
{
	lastPosition = position;
	position = newPosition;
}

bool Movable::getMoved() const
{
	return canMove && (moved || lastPosition != position);
}

bool Movable::getIsMoving() const
{
	return isMoving;
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

void Movable::startMoving()
{
	if (isMoving)
		return;
	isMoving = true;
	runningAnimation->reset();
}

void Movable::stopMoving()
{
	isMoving = false;
}

bool Movable::hasPlayableAnimation() const
{
	return (moved && runningAnimation != nullptr) ||
	       (!grounded && ((v.y > 0 && airDownAnimation != nullptr) || (v.y <= 0 && airUpAnimation != nullptr)));
}

sdl::Sprite Movable::updateAnimation(sdl::GameClock::duration frameDelta)
{
	if (!grounded) {
		if (v.y > 0 && airDownAnimation != nullptr) {
			auto numFrames = airDownAnimation->getFrameCount();
			auto scaledVelocity = (float)v.y / (2 * maxSpeed);
			int index = numFrames * scaledVelocity;
			return airDownAnimation->getSprite(index);
		} else if (airUpAnimation != nullptr) {
			auto numFrames = airUpAnimation->getFrameCount();
			auto scaledVelocity = (float)-v.y / (2 * maxSpeed);
			int index = numFrames * (1 - scaledVelocity);
			return airUpAnimation->getSprite(index);
		}
	}

	return runningAnimation->updateAnimation(frameDelta);
}
