#ifndef PHYSICS_H
#define PHYSICS_H

#include "Game.hpp"

namespace game {

inline bool intersects(Rectangle lhs, Rectangle rhs)
{
	return sdl::intersects(lhs, rhs);
}

bool collidesLeft(Rectangle playerHitBox, Room &currentRoom);

bool collidesRight(Rectangle playerHitBox, Room &currentRoom);

bool collidesTop(Rectangle playerHitBox, Room &currentRoom);

bool collidesBottom(Rectangle playerHitBox, Room &currentRoom, Collision testCollision = Collision::Full);

bool isStanding(Rectangle playerHitBox, Room &currentRoom);

void resolveRoomCollision(Player &player, Room &currentRoom); // TODO make this take a Movable

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
