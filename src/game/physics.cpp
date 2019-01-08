#include "game/physics.hpp"

using namespace game;
using namespace sdl;

bool game::collidesLeft(Rectangle playerHitBox, Room &currentRoom)
{
	auto j = playerHitBox.x / tileSize.w;
	auto lowestRowIndex = playerHitBox.y / tileSize.h;
	auto highestRowIndex = (playerHitBox.y + playerHitBox.h) / tileSize.h;
	for (int i = lowestRowIndex; i < highestRowIndex; ++i) {
		if (currentRoom.collisionMap[i][j] == Collision::Full &&
		    sdl::intersects_left(playerHitBox, tileRectangle(i, j)))
			return true;
	}
	// Check against left border
	if (playerHitBox.x < 0)
		return true;
	return false;
}

bool game::collidesRight(Rectangle playerHitBox, Room &currentRoom)
{
	auto j = (playerHitBox.x + playerHitBox.w) / tileSize.w;
	auto lowestRowIndex = playerHitBox.y / tileSize.h;
	auto highestRowIndex = (playerHitBox.y + playerHitBox.h) / tileSize.h;
	for (int i = lowestRowIndex; i < highestRowIndex; ++i) {
		if (currentRoom.collisionMap[i][j] == Collision::Full &&
		    sdl::intersects_right(playerHitBox, tileRectangle(i, j)))
			return true;
	}
	// Check against right border
	if (playerHitBox.x + playerHitBox.w > currentRoom.sizeInPixels.w)
		return true;
	return false;
}

bool game::collidesTop(Rectangle playerHitBox, Room &currentRoom)
{
	auto i = getTileRow(playerHitBox);
	auto lowestColumnIndex = getLowestTileColumn(playerHitBox);
	auto highestColumnIndex = getHighestTileColumn(playerHitBox);
	for (int j = lowestColumnIndex; j < highestColumnIndex; ++j) {
		if (currentRoom.collisionMap[i][j] == Collision::Full && sdl::intersects_top(playerHitBox, tileRectangle(i, j)))
			return true;
	}
	return false;
}

bool game::collidesBottom(Rectangle playerHitBox, Room &currentRoom, Collision testCollision)
{
	auto i = getTileRow(playerHitBox);
	auto lowestColumnIndex = getLowestTileColumn(playerHitBox);
	auto highestColumnIndex = getHighestTileColumn(playerHitBox);
	for (int j = lowestColumnIndex; j < highestColumnIndex; ++j) {
		if (currentRoom.collisionMap[i][j] == testCollision &&
		    sdl::intersects_bottom(playerHitBox, tileRectangle(i, j)))
			return true;
	}
	return false;
}

bool game::isStanding(Rectangle playerHitBox, Room &currentRoom)
{
	auto i = getTileRow(playerHitBox);
	auto lowestColumnIndex = getLowestTileColumn(playerHitBox);
	auto highestColumnIndex = getHighestTileColumn(playerHitBox);
	for (int j = lowestColumnIndex; j < highestColumnIndex; ++j) {
		if (currentRoom.collisionMap[i][j] > Collision::None && touches_bottom(playerHitBox, tileRectangle(i, j)))
			return true;
	}
	return false;
}

void game::resolveRoomCollision(Entity &entity, Room &currentRoom)
{
	// first resolve collisions with the room
	auto hitBox = entity.calcPositionedHitbox();
	auto lastHitBox = entity.calcLastPositionedHitbox();
	auto moveDirection = entity.movable.getDirection();

	if (!isStanding(hitBox, currentRoom))
		entity.movable.grounded = false;

	if ((collidesLeft(hitBox, currentRoom) && collidesRight(hitBox, currentRoom)) ||
	    (collidesTop(hitBox, currentRoom) && collidesBottom(hitBox, currentRoom)))
		entity.movable.canMove = false;

	if (entity.movable.canMove && entity.movable.getMoved()) {
		while (collidesLeft(hitBox, currentRoom)) {
			auto newPosition = entity.movable.getPosition() + Point{1, 0};
			entity.movable.reposition(newPosition);
			hitBox = entity.calcPositionedHitbox();
		}
		// hitBox = player.calcPositionedHitbox();

		while (collidesRight(hitBox, currentRoom)) {
			auto newPosition = entity.movable.getPosition() + Point{-1, 0};
			entity.movable.reposition(newPosition);
			hitBox = entity.calcPositionedHitbox();
		}
		// hitBox = player.calcPositionedHitbox();

		while (moveDirection.y > 0 && collidesBottom(hitBox, currentRoom)) {
			entity.movable.grounded = true;
			entity.movable.v.y = 0;
			auto newPosition = entity.movable.getPosition() + Point{0, -1};
			entity.movable.reposition(newPosition);
			hitBox = entity.calcPositionedHitbox();
		}
		// hitBox = player.calcPositionedHitbox();

		while (collidesTop(hitBox, currentRoom)) {
			auto newPosition = entity.movable.getPosition() + Point{0, 1};
			entity.movable.reposition(newPosition);
			hitBox = entity.calcPositionedHitbox();
		}

    //plaftorms
		while (moveDirection.y > 0 && !entity.movable.fallThroughPlatforms &&
		       !collidesBottom(lastHitBox, currentRoom, Collision::TopOnly) &&
		       collidesBottom(hitBox, currentRoom, Collision::TopOnly)) {
			entity.movable.grounded = true;
			entity.movable.v.y = 0;
			auto newPosition = entity.movable.getPosition() + Point{0, -1};
			entity.movable.reposition(newPosition);
			hitBox = entity.calcPositionedHitbox();
		}
	}
}
