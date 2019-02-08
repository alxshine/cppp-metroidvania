#include "game/attacks.hpp"

using namespace game;

Rectangle Projectile::calcPositionedHitbox()
{
	return {position.x + hitbox.w / 2, position.y + hitbox.h / 2, hitbox.w, hitbox.h};
}

int Projectile::update(sdl::GameClock::duration frameDelta, std::vector<Rectangle> hitboxes,
                       const CollisionMap &collisionMap)
{
	position += v * frameDelta;
	auto phb = calcPositionedHitbox();
	if (v.x >= 0)
		done = collidesRight(phb, collisionMap);
	else
		done = collidesLeft(phb, collisionMap);

	for (unsigned i = 0; i < hitboxes.size(); ++i) {
		const auto &hitbox = hitboxes[i];
		if (intersects(phb, hitbox)) {
			done = true;
			return i;
		}
	}

	return -1;
}

sdl::Sprite Projectile::updateAnimation(sdl::GameClock::duration frameDelta)
{
	return animation.updateAnimation(frameDelta);
}
