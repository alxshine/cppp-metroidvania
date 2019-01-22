#include "game/attacks.hpp"

using namespace game;

Rectangle Projectile::calcPositionedHitbox()
{
	return {position.x + hitbox.w / 2, position.y + hitbox.h / 2, hitbox.w, hitbox.h};
}

/**
 * @brief update the projectile and set it to be done if it collides with something
 *
 * @return true if the player was hit
 **/
bool Projectile::update(sdl::GameClock::duration frameDelta, Rectangle playerHitbox, const CollisionMap &collisionMap)
{
	position += v * frameDelta;
	auto phb = calcPositionedHitbox();
	if (v.x >= 0)
		done = collidesRight(phb, collisionMap);
	else
		done = collidesLeft(phb, collisionMap);

	if (intersects(phb, playerHitbox)) {
		done = true;
		return true;
	}
	return false;
}

sdl::Sprite Projectile::updateAnimation(sdl::GameClock::duration frameDelta){
  return animation.updateAnimation(frameDelta);
}
