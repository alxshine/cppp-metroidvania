#ifndef PHYSICS_H
#define PHYSICS_H

#include "game/Movable.hpp"
#include "game/constants.hpp"

namespace game {

bool collidesLeft(Rectangle hitbox, const CollisionMap &currentRoom);

bool collidesRight(Rectangle hitbox, const CollisionMap &currentRoom);

bool collidesTop(Rectangle hitbox, const CollisionMap &currentRoom);

bool collidesBottom(Rectangle hitbox, const CollisionMap &currentRoom, Collision testCollision = Collision::Full);

bool isStanding(Rectangle hitbox, const CollisionMap &currentRoom);

void resolveRoomCollision(Movable &movable, const CollisionMap &currentRoom);

inline int getTopTileRow(Rectangle hitbox)
{
	return hitbox.y / tileSize.h;
}

inline int getTileRow(Rectangle hitbox)
{
	return (hitbox.y + hitbox.h) / tileSize.h;
}

inline int getLowestTileColumn(Rectangle hitbox)
{
	return hitbox.x / tileSize.w;
}

inline int getHighestTileColumn(Rectangle hitbox)
{
	return (hitbox.x + 2 * hitbox.w) / tileSize.w;
}

} // namespace game

#endif /* PHYSICS_H */
