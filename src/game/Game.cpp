#include "game/Game.hpp"

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

static Position calcCameraPosition(const Entity &player, const Room &room, const Renderer &renderer)
{
	const Position p = player.movable.getPosition();
	const Rectangle rs = room.sizeInPixels;
	Position camera = {0, std::max(rs.h - renderer.logicalH, 0)};

	// Only scroll when the room is wider/higher than the screen
	// AND the player is more than half inside it.

	if (p.x >= 0.5 * renderer.logicalW)
		camera.x = std::min(p.x - renderer.logicalW / 2, rs.w - renderer.logicalW);
	if (p.y <= 0.5 * renderer.logicalH)
		camera.y = std::max(p.y - renderer.logicalH / 2, 0);
	return camera;
}

void Game::initialize()
{
	lastGameFrameTime = gameClock.now();
}

void Game::runMainLoop()
{
	while (running) {
		// will need to decide between game and menu stack main loop here
		renderer.clear();
		renderer.setCameraPosition(calcCameraPosition(*player, *currentRoom, renderer));
		auto now = gameClock.now();
		gameFrameDelta = now - lastGameFrameTime;

		// reset player velocity
		player->movable.v.x = 0;
		player->movable.fallThroughPlatforms = false;

		// events
		gameEvents.dispatch();

		// gravity
		player->movable.applyGravity(gameFrameDelta);

		// Room
		// currentRoom->update()

		// Player
		player->movable.update(gameFrameDelta);
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
	gameEvents.whileKeyHeld(SDL_SCANCODE_S, [this]() { player->movable.fall(); });
	// jump
	gameEvents.whileKeyHeld(SDL_SCANCODE_SPACE, [this]() { player->movable.jump(); });

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
