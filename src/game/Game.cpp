#include "game/Game.hpp"

using namespace game;
using namespace sdl;

Game::Game(std::string definitions, std::string assets, std::string first_room, Position player_position,
           sdl::RenderOptions renderOpts)
    : res(definitions, assets), currentRoom(res.getRoom(first_room)), player(res.makePlayer()), renderOpts(renderOpts)
{
	player->movable.reposition(player_position);
	play(currentRoom.music, repeat_forever);
	registerGameEvents();
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

bool collidesBottom(Rectangle playerHitBox, Room currentRoom)
{
	auto i = (playerHitBox.y + playerHitBox.h) / tileSize.h;
	auto lowestColumnIndex = playerHitBox.x / tileSize.w;
	auto highestColumnIndex = (playerHitBox.x + playerHitBox.w) / tileSize.w;
	for (int j = lowestColumnIndex; j < highestColumnIndex; ++j) {
		if (currentRoom.collisionMap[i][j] == Collision::Full &&
		    sdl::intersects_bottom(playerHitBox, tileRectangle(i, j)))
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

void Game::runMainLoop()
{
	while (running) {
		// will need to decide between game and menu stack main loop here
		renderer.clear();
		auto now = gameClock.now();
		game_frame_delta = now - last_game_frame_time;

		player->movable.v.x = 0;
		gameEvents.dispatch();

		// HUD

		// Room
		// currentRoom.update()
		renderer.render(currentRoom, now, renderOpts);

		// Player
		player->movable.move(game_frame_delta);
		player->movable.update();
		renderer.render(*player, now, renderOpts);

		// Collision
		// first resolve collisions with the room
		auto playerHitBox = player->calcPositionedHitbox();

		if ((collidesLeft(playerHitBox, currentRoom) && collidesRight(playerHitBox, currentRoom)) ||
		    (collidesTop(playerHitBox, currentRoom) && collidesBottom(playerHitBox, currentRoom)))
			player->movable.canMove = false;

		if (player->movable.canMove && player->movable.getMoved()) {

			while (collidesLeft(playerHitBox, currentRoom)) {
				auto newPosition = player->movable.getPosition() + Point{1, 0};
				player->movable.reposition(newPosition);
				playerHitBox = player->calcPositionedHitbox();
			}

			while (collidesRight(playerHitBox, currentRoom)) {
				auto newPosition = player->movable.getPosition() + Point{-1, 0};
				player->movable.reposition(newPosition);
				playerHitBox = player->calcPositionedHitbox();
			}

			while (collidesBottom(playerHitBox, currentRoom)) {
				auto newPosition = player->movable.getPosition() + Point{0, -1};
				player->movable.reposition(newPosition);
				playerHitBox = player->calcPositionedHitbox();
			}

			while (collidesTop(playerHitBox, currentRoom)) {
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
	});
	gameEvents.whileKeyHeld(SDL_SCANCODE_D, [this]() {player->movable.v.x = player->movable.maxSpeed;});
	gameEvents.whileKeyHeld(SDL_SCANCODE_A, [this]() {player->movable.v.x = -player->movable.maxSpeed;});
}
