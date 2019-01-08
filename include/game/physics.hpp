#ifndef PHYSICS_H
#define PHYSICS_H

#include "Game.hpp"

namespace game {

bool collidesLeft(Rectangle playerHitBox, Room &currentRoom);

bool collidesRight(Rectangle playerHitBox, Room &currentRoom);

bool collidesTop(Rectangle playerHitBox, Room &currentRoom);

bool collidesBottom(Rectangle playerHitBox, Room &currentRoom, Collision testCollision = Collision::Full);

bool isStanding(Rectangle playerHitBox, Room &currentRoom);

void resolvePlayerCollision(Entity &player, Room &currentRoom);

inline int getTileRow(Rectangle playerHitBox)
{
	return (playerHitBox.y + playerHitBox.h) / tileSize.h;
}

inline int getLowestTileColumn(Rectangle playerHitBox)
{
	return playerHitBox.x / tileSize.w;
}

inline int getHighestTileColumn(Rectangle playerHitBox)
{
	return (playerHitBox.x + 2 * playerHitBox.w) / tileSize.w;
}

} // namespace game

#endif /* PHYSICS_H */
