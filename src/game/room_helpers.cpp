#include "game/room_helpers.hpp"

using namespace game;
using namespace sdl;

bool game::collidesLeft(Rectangle hitbox, const CollisionMap &collisionMap)
{
	auto j = hitbox.x / tileSize.w;
	auto lowestRowIndex = hitbox.y / tileSize.h;
	auto highestRowIndex = (hitbox.y + hitbox.h) / tileSize.h;
	for (int i = lowestRowIndex; i < highestRowIndex; ++i) {
		if (collisionMap[i][j] == Collision::Full && sdl::intersects_left(hitbox, tileRectangle(i, j)))
			return true;
	}
	// Check against left border
	if (hitbox.x < 0)
		return true;
	return false;
}

bool game::collidesRight(Rectangle hitbox, const CollisionMap &collisionMap)
{
	auto j = (hitbox.x + hitbox.w) / tileSize.w;
	auto lowestRowIndex = hitbox.y / tileSize.h;
	auto highestRowIndex = (hitbox.y + hitbox.h) / tileSize.h;
	for (int i = lowestRowIndex; i < highestRowIndex; ++i) {
		if (collisionMap[i][j] == Collision::Full && sdl::intersects_right(hitbox, tileRectangle(i, j)))
			return true;
	}
	// Check against right border
	if (hitbox.x + hitbox.w > (int)collisionMap[0].size() * game::tileSize.w)
		return true;
	return false;
}

bool game::collidesTop(Rectangle hitbox, const CollisionMap &collisionMap)
{
	auto i = getTopTileRow(hitbox);
	auto lowestColumnIndex = getLowestTileColumn(hitbox);
	auto highestColumnIndex = getHighestTileColumn(hitbox);
	for (int j = lowestColumnIndex; j < highestColumnIndex; ++j) {
		if (collisionMap[i][j] == Collision::Full && sdl::intersects_top(hitbox, tileRectangle(i, j)))
			return true;
	}
	return false;
}

bool game::collidesBottom(Rectangle hitbox, const CollisionMap &collisionMap, Collision testCollision)
{
	auto i = getTileRow(hitbox);
	auto lowestColumnIndex = getLowestTileColumn(hitbox);
	auto highestColumnIndex = getHighestTileColumn(hitbox);
	for (int j = lowestColumnIndex; j < highestColumnIndex; ++j) {
		if (collisionMap[i][j] == testCollision && sdl::intersects_bottom(hitbox, tileRectangle(i, j)))
			return true;
	}
	return false;
}

bool game::isStanding(Rectangle hitbox, const CollisionMap &collisionMap)
{
	auto i = getTileRow(hitbox);
	auto lowestColumnIndex = getLowestTileColumn(hitbox);
	auto highestColumnIndex = getHighestTileColumn(hitbox);
	for (int j = lowestColumnIndex; j < highestColumnIndex; ++j) {
		if (collisionMap[i][j] > Collision::None && touches_bottom(hitbox, tileRectangle(i, j)))
			return true;
	}
	return false;
}

void game::resolveRoomCollision(Movable &movable, const CollisionMap &collisionMap)
{
	// first resolve collisions with the room
	auto hitBox = movable.calcPositionedHitbox();
	auto lastHitBox = movable.calcLastPositionedHitbox();
	auto moveDirection = movable.getDirection();

	if (!isStanding(hitBox, collisionMap))
		movable.grounded = false;

	// if ((collidesLeft(hitBox, collisionMap) && collidesRight(hitBox, collisionMap)) ||
	//     (collidesTop(hitBox, collisionMap) && collidesBottom(hitBox, collisionMap)))
	// 	movable.canMove = false;

	if (movable.canMove && movable.getMoved()) {
		while (collidesTop(hitBox, collisionMap)) {
			auto newPosition = movable.getPosition() + Point{0, 1};
			movable.reposition(newPosition);
			hitBox = movable.calcPositionedHitbox();
		}

		while (collidesLeft(hitBox, collisionMap)) {
			auto newPosition = movable.getPosition() + Point{1, 0};
			movable.reposition(newPosition);
			hitBox = movable.calcPositionedHitbox();
		}
		// hitBox = player.calcPositionedHitbox();

		while (collidesRight(hitBox, collisionMap)) {
			auto newPosition = movable.getPosition() + Point{-1, 0};
			movable.reposition(newPosition);
			hitBox = movable.calcPositionedHitbox();
		}
		// hitBox = player.calcPositionedHitbox();

		while (moveDirection.y > 0 && collidesBottom(hitBox, collisionMap)) {
			movable.setGrounded();
			movable.v.y = 0;
			auto newPosition = movable.getPosition() + Point{0, -1};
			movable.reposition(newPosition);
			hitBox = movable.calcPositionedHitbox();
		}
		// hitBox = player.calcPositionedHitbox();

		// platforms
		while (moveDirection.y > 0 && !movable.fallThroughPlatforms &&
		       !collidesBottom(lastHitBox, collisionMap, Collision::TopOnly) &&
		       collidesBottom(hitBox, collisionMap, Collision::TopOnly)) {
			movable.setGrounded();
			movable.v.y = 0;
			auto newPosition = movable.getPosition() + Point{0, -1};
			movable.reposition(newPosition);
			hitBox = movable.calcPositionedHitbox();
		}
	}
}
