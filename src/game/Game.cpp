#include "game/Game.hpp"

#include <algorithm>
#include <iostream>

using namespace game;
using namespace sdl;

Game::Game(std::string definitions, std::string assets, std::string first_room, Position player_position,
           sdl::RenderOptions renderOpts)
    : res(definitions, assets), currentRoom(std::make_unique<Room>(res.getRoom(first_room))), player(res.makePlayer()),
      renderOpts(renderOpts)
{
	player->movable.reposition(player_position);
	play(currentRoom->music, repeat_forever);
	registerGameEvents();
}

void Game::interact()
{
	Rectangle playerHitbox = player->calcPositionedHitbox();

	// Find item to interact with
	for (auto &i : currentRoom->items) {
		if (intersects(playerHitbox, i.calcPositionedHitbox())) {
			std::cout << "Player interacted with " << i.name << std::endl;
			return;
		}
	}

	// Find door to interact with
	for (auto door : currentRoom->doors) {
		if (intersects(playerHitbox, door.item.calcPositionedHitbox())) {
			std::cout << "Player went to " << door.targetRoom << " through " << door.name << std::endl;

			// this conditions allows portals that don't reset room state
			if (currentRoom->name != door.targetRoom)
				currentRoom = std::make_unique<Room>(res.getRoom(door.targetRoom));

			auto newDoorIt = std::find_if(currentRoom->doors.cbegin(), currentRoom->doors.cend(),
			                              [door](Door d) { return d.name == door.targetDoorName; });
			if (newDoorIt == currentRoom->doors.cend()) {
				std::cerr << "No door named " << door.targetDoorName << " in room " << door.targetRoom << std::endl;
			} else {
				auto newPosition = newDoorIt->item.movable.getPosition();
				newPosition.x += player->hitbox.w / 2;
				player->movable.reposition(newPosition);
				player->movable.setDirection(door.direction);
			}

			return;
		}
	}
}

Rectangle tileRectangle(int row, int column)
{
	return {column * tileSize.w, row * tileSize.h, tileSize.w, tileSize.h};
}

bool collidesLeft(Rectangle playerHitBox, Room currentRoom)
{
	auto j = playerHitBox.x / tileSize.w;
	auto lowestRowIndex = playerHitBox.y / tileSize.h;
	auto highestRowIndex = (playerHitBox.y + playerHitBox.h) / tileSize.h;
	for (int i = lowestRowIndex; i < highestRowIndex; ++i) {
		if (currentRoom.collisionMap[i][j] == Collision::Full &&
		    sdl::intersects_left(playerHitBox, tileRectangle(i, j)))
			return true;
	}
	return false;
}

bool collidesRight(Rectangle playerHitBox, Room currentRoom)
{
	auto j = (playerHitBox.x + playerHitBox.w) / tileSize.w;
	auto lowestRowIndex = playerHitBox.y / tileSize.h;
	auto highestRowIndex = (playerHitBox.y + playerHitBox.h) / tileSize.h;
	for (int i = lowestRowIndex; i < highestRowIndex; ++i) {
		if (currentRoom.collisionMap[i][j] == Collision::Full &&
		    sdl::intersects_right(playerHitBox, tileRectangle(i, j)))
			return true;
	}
	return false;
}

bool collidesTop(Rectangle playerHitBox, Room currentRoom)
{
	auto i = playerHitBox.y / tileSize.h;
	auto lowestColumnIndex = playerHitBox.x / tileSize.w;
	auto highestColumnIndex = (playerHitBox.x + playerHitBox.w) / tileSize.w;
	for (int j = lowestColumnIndex; j < highestColumnIndex; ++j) {
		if (currentRoom.collisionMap[i][j] == Collision::Full && sdl::intersects_top(playerHitBox, tileRectangle(i, j)))
			return true;
	}
	return false;
}

bool collidesBottom(Rectangle playerHitBox, Room currentRoom)
{
	auto i = (playerHitBox.y + playerHitBox.h) / tileSize.h;
	auto lowestColumnIndex = playerHitBox.x / tileSize.w;
	auto highestColumnIndex = (playerHitBox.x + playerHitBox.w) / tileSize.w;
	for (int j = lowestColumnIndex; j < highestColumnIndex; ++j) {
		if ((currentRoom.collisionMap[i][j] == Collision::Full ||
		     currentRoom.collisionMap[i][j] == Collision::TopOnly) &&
		    sdl::intersects_bottom(playerHitBox, tileRectangle(i, j)))
			return true;
	}
	return false;
}

inline bool isStanding(Rectangle playerHitBox, Room currentRoom)
{
	auto i = (playerHitBox.y + playerHitBox.h) / tileSize.h;
	auto lowestColumnIndex = playerHitBox.x / tileSize.w;
	auto highestColumnIndex = (playerHitBox.x + playerHitBox.w) / tileSize.w;
	for (int j = lowestColumnIndex; j < highestColumnIndex; ++j) {
		if (currentRoom.collisionMap[i][j] > Collision::None && touches_bottom(playerHitBox, tileRectangle(i, j)))
			return true;
	}
	return false;
}

void Game::runMainLoop()
{
	while (running) {
		// will need to decide between game and menu stack main loop here
		renderer.clear();
		auto now = gameClock.now();
		game_frame_delta = now - last_game_frame_time;

		player->movable.v.x = 0;
		if (!player->movable.grounded)
			player->movable.v.y += 10;
		gameEvents.dispatch();

		// HUD

		// Room
		// currentRoom.update()
		renderer.render(*currentRoom, now, renderOpts);

		// Player
		player->movable.move(game_frame_delta);
		player->movable.update();
		renderer.render(*player, now, renderOpts);

		// Collision
		// first resolve collisions with the room
		auto playerHitBox = player->calcPositionedHitbox();

		if (!isStanding(playerHitBox, *currentRoom))
			player->movable.grounded = false;

		if ((collidesLeft(playerHitBox, *currentRoom) && collidesRight(playerHitBox, *currentRoom)) ||
		    (collidesTop(playerHitBox, *currentRoom) && collidesBottom(playerHitBox, *currentRoom)))
			player->movable.canMove = false;

		if (player->movable.canMove && player->movable.getMoved()) {

			while (collidesLeft(playerHitBox, *currentRoom)) {
				auto newPosition = player->movable.getPosition() + Point{1, 0};
				player->movable.reposition(newPosition);
				playerHitBox = player->calcPositionedHitbox();
			}

			while (collidesRight(playerHitBox, *currentRoom)) {
				auto newPosition = player->movable.getPosition() + Point{-1, 0};
				player->movable.reposition(newPosition);
				playerHitBox = player->calcPositionedHitbox();
			}

			while (collidesBottom(playerHitBox, *currentRoom)) {
				player->movable.grounded = true;
				player->movable.v.y = 0;
				auto newPosition = player->movable.getPosition() + Point{0, -1};
				player->movable.reposition(newPosition);
				playerHitBox = player->calcPositionedHitbox();
			}

			while (collidesTop(playerHitBox, *currentRoom)) {
				auto newPosition = player->movable.getPosition() + Point{0, 1};
				player->movable.reposition(newPosition);
				playerHitBox = player->calcPositionedHitbox();
			}
		}

		last_game_frame_time = now;
		renderer.swapBuffers();
	}

	music::fade_out(1s);
	music::block_until_stopped();
}

void Game::registerGameEvents()
{

	gameEvents.on(SDL_QUIT, [this](const Event &) { running = false; });
	gameEvents.onKeyDown(SDLK_ESCAPE, [this](const KeyboardEvent &) { running = false; });
	gameEvents.onKeyDown(SDLK_c, [this](const KeyboardEvent &) {
		renderOpts.renderCollisionMap = !renderOpts.renderCollisionMap;
		renderOpts.renderEntityDrawRectangles = !renderOpts.renderEntityDrawRectangles;
		renderOpts.renderHitBoxes = !renderOpts.renderHitBoxes;
	});
	gameEvents.onKeyDown(SDLK_e, [this](const KeyboardEvent &) { this->interact(); });

	gameEvents.onKeyDown(SDLK_SPACE, [this](const KeyboardEvent &) {
		if (player->movable.grounded) {
			player->movable.v.y = -player->movable.maxSpeed;
			player->movable.grounded = false;
		}
	});

	gameEvents.onKeyDown(SDLK_j, [this](const KeyboardEvent &) {
		const Uint8 *keyHeld = SDL_GetKeyboardState(nullptr);
		Position pos = player->movable.getPosition();
		if (keyHeld[SDL_SCANCODE_W])
			player->movable.reposition({pos.x, pos.y - 80});
		if (keyHeld[SDL_SCANCODE_S])
			player->movable.reposition({pos.x, pos.y + 80});
		if (keyHeld[SDL_SCANCODE_A])
			player->movable.reposition({pos.x - 80, pos.y});
		if (keyHeld[SDL_SCANCODE_D])
			player->movable.reposition({pos.x + 80, pos.y});

		player->movable.canMove = true;
	});
	gameEvents.whileKeyHeld(SDL_SCANCODE_D, [this]() { player->movable.v.x = player->movable.maxSpeed; });
	gameEvents.whileKeyHeld(SDL_SCANCODE_A, [this]() { player->movable.v.x = -player->movable.maxSpeed; });
}
