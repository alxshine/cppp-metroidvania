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

bool game::collidesBottom(Rectangle playerHitBox, Room &currentRoom)
{
	auto i = getTileRow(playerHitBox);
	auto lowestColumnIndex = getLowestTileColumn(playerHitBox);
	auto highestColumnIndex = getHighestTileColumn(playerHitBox);
	for (int j = lowestColumnIndex; j < highestColumnIndex; ++j) {
		if ((currentRoom.collisionMap[i][j] == Collision::Full ||
		     currentRoom.collisionMap[i][j] == Collision::TopOnly) &&
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

void game::resolvePlayerCollision(Player &player, Room &currentRoom)
{
	// first resolve collisions with the room
	auto playerHitBox = player.calcPositionedHitbox();

	if (!isStanding(playerHitBox, currentRoom))
		player.movable.grounded = false;

	if ((collidesLeft(playerHitBox, currentRoom) && collidesRight(playerHitBox, currentRoom)) ||
	    (collidesTop(playerHitBox, currentRoom) && collidesBottom(playerHitBox, currentRoom)))
		player.movable.canMove = false;

	if (player.movable.canMove && player.movable.getMoved()) {

		while (collidesLeft(playerHitBox, currentRoom)) {
			auto newPosition = player.movable.getPosition() + Point{1, 0};
			player.movable.reposition(newPosition);
			playerHitBox = player.calcPositionedHitbox();
		}

		while (collidesRight(playerHitBox, currentRoom)) {
			auto newPosition = player.movable.getPosition() + Point{-1, 0};
			player.movable.reposition(newPosition);
			playerHitBox = player.calcPositionedHitbox();
		}

		while (collidesBottom(playerHitBox, currentRoom)) {
			player.movable.grounded = true;
			player.movable.v.y = 0;
			auto newPosition = player.movable.getPosition() + Point{0, -1};
			player.movable.reposition(newPosition);
			playerHitBox = player.calcPositionedHitbox();
		}

		while (collidesTop(playerHitBox, currentRoom)) {
			auto newPosition = player.movable.getPosition() + Point{0, 1};
			player.movable.reposition(newPosition);
			playerHitBox = player.calcPositionedHitbox();
		}
	}
}
