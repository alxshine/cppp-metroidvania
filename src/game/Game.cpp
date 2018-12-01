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

void Game::runMainLoop()
{
	while (running) {
		// will need to decide between game and menu stack main loop here
		renderer.clear();
		auto now = gameClock.now();
		game_frame_delta = now - last_game_frame_time;

		gameEvents.dispatch();

		// HUD

		// Room
		// currentRoom.update()
		renderer.render(currentRoom, now, renderOpts);

		// Player
		player->movable.update();
		renderer.render(*player, now, renderOpts);

		// Collision
		//first resolve collisions with the room
		auto playerHitBox = player->calcPositionedHitbox();
		//check if hitbox intersects something for all sides
		//if it also intersects something on the opposite side, do nothing -> movable = false
		//if it doesn't, move it until it doesn't intersect anymore
		//if it starts to intersect something on the other side during movement, stop moving -> movable = false

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
	gameEvents.whileKeyHeld(SDL_SCANCODE_D, [this]() { player->movable.move({1, 0}, game_frame_delta); });
	gameEvents.whileKeyHeld(SDL_SCANCODE_A, [this]() { player->movable.move({-1, 0}, game_frame_delta); });
}
