#include "game/Game.hpp"
#include "game/physics.hpp"

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
			if (currentRoom->name != door.targetRoom) {
				currentRoom = std::make_unique<Room>(res.getRoom(door.targetRoom));
				play(currentRoom->music, repeat_forever);
			}

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

void Game::runMainLoop()
{
	while (running) {
		// will need to decide between game and menu stack main loop here
		renderer.clear();
		auto now = gameClock.now();
		gameFrameDelta = now - lastGameFrameTime;

		// reset player velocity
		player->movable.v.x = 0;
		if (!player->movable.grounded) // gravity
			player->movable.v.y += 10;

		// events
		gameEvents.dispatch();

		// Room
		// currentRoom->update()

		// Player
		player->movable.move(gameFrameDelta);
		player->movable.update();
		resolvePlayerCollision(*player, *currentRoom);

		// render
		renderer.render(*currentRoom, now, renderOpts);
		renderer.render(*player, now, renderOpts);

		lastGameFrameTime = now;
		renderer.swapBuffers();
	}

	music::fade_out(1s);
	music::block_until_stopped();
}

void Game::registerGameEvents()
{
	// quit
	gameEvents.on(SDL_QUIT, [this](const Event &) { running = false; });
	gameEvents.onKeyDown(SDLK_ESCAPE, [this](const KeyboardEvent &) { running = false; });
	// debug overlay
	gameEvents.onKeyDown(SDLK_c, [this](const KeyboardEvent &) {
		renderOpts.renderCollisionMap = !renderOpts.renderCollisionMap;
		renderOpts.renderEntityDrawRectangles = !renderOpts.renderEntityDrawRectangles;
		renderOpts.renderHitBoxes = !renderOpts.renderHitBoxes;
	});

	// jump down from platforms
	gameEvents.whileKeyHeld(SDL_SCANCODE_S, [this]() {
		auto hitbox = player->calcPositionedHitbox();
		auto i = getTileRow(hitbox);
		auto min_j = getLowestTileColumn(hitbox);
		auto max_j = getHighestTileColumn(hitbox);
		for (int j = min_j; j < max_j; ++j)
			if (currentRoom->collisionMap[i][j] == Collision::TopOnly) {
				player->movable.reposition(player->movable.getPosition() + Point{0, hitbox.h});
				player->movable.v.y += 10;
				return;
			}
	});
	// jump
	gameEvents.whileKeyHeld(SDL_SCANCODE_SPACE, [this]() {
		if (player->movable.grounded) {
			player->movable.v.y = -2 * player->movable.maxSpeed;
			player->movable.grounded = false;
		}
	});

	// interaction
	gameEvents.onKeyDown(SDLK_e, [this](const KeyboardEvent &) { this->interact(); });

	// blink
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

	// normal movement
	gameEvents.whileKeyHeld(SDL_SCANCODE_D, [this]() { player->movable.v.x = player->movable.maxSpeed; });
	gameEvents.whileKeyHeld(SDL_SCANCODE_A, [this]() { player->movable.v.x = -player->movable.maxSpeed; });
}
