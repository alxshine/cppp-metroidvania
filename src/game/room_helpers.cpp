#include "game/room_helpers.hpp"

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

void game::resolveRoomCollision(Movable &movable, Room &currentRoom)
{
	// first resolve collisions with the room
	auto hitBox = movable.calcPositionedHitbox();
	auto lastHitBox = movable.calcLastPositionedHitbox();
	auto moveDirection = movable.getDirection();

	if (!isStanding(hitBox, currentRoom))
		movable.grounded = false;

	if ((collidesLeft(hitBox, currentRoom) && collidesRight(hitBox, currentRoom)) ||
	    (collidesTop(hitBox, currentRoom) && collidesBottom(hitBox, currentRoom)))
		movable.canMove = false;

	if (movable.canMove && movable.getMoved()) {
		while (collidesLeft(hitBox, currentRoom)) {
			auto newPosition = movable.getPosition() + Point{1, 0};
			movable.reposition(newPosition);
			hitBox = movable.calcPositionedHitbox();
		}
		// hitBox = player.calcPositionedHitbox();

		while (collidesRight(hitBox, currentRoom)) {
			auto newPosition = movable.getPosition() + Point{-1, 0};
			movable.reposition(newPosition);
			hitBox = movable.calcPositionedHitbox();
		}
		// hitBox = player.calcPositionedHitbox();

		while (moveDirection.y > 0 && collidesBottom(hitBox, currentRoom)) {
			movable.grounded = true;
			movable.v.y = 0;
			auto newPosition = movable.getPosition() + Point{0, -1};
			movable.reposition(newPosition);
			hitBox = movable.calcPositionedHitbox();
		}
		// hitBox = player.calcPositionedHitbox();

		while (collidesTop(hitBox, currentRoom)) {
			auto newPosition = movable.getPosition() + Point{0, 1};
			movable.reposition(newPosition);
			hitBox = movable.calcPositionedHitbox();
		}

    //plaftorms
		while (moveDirection.y > 0 && !movable.fallThroughPlatforms &&
		       !collidesBottom(lastHitBox, currentRoom, Collision::TopOnly) &&
		       collidesBottom(hitBox, currentRoom, Collision::TopOnly)) {
			movable.grounded = true;
			movable.v.y = 0;
			auto newPosition = movable.getPosition() + Point{0, -1};
			movable.reposition(newPosition);
			hitBox = movable.calcPositionedHitbox();
		}
	}
}
