#include "game/Game.hpp"

#include <chrono>
#include <ctime>
#include <filesystem>

using namespace game;
using namespace menu;
using namespace sdl;
using namespace std::literals;
namespace fs = std::filesystem;

Game::Game(std::string definitions, std::string assets, std::string first_room, Position player_position,
           sdl::RenderOptions renderOpts)
    : res(definitions, assets), firstRoom(first_room), initialPosition(player_position),
      currentRoom(std::make_unique<Room>(res.getRoom(first_room))), player(res.makePlayer()), renderOpts(renderOpts)
{
	registerGameEvents();

	std::vector<RawMenuItem> mainMenuItems = {{"New Game",
	                                           [&]() {
		                                           music::fade_out(500ms);
		                                           resetState();
		                                           menuStack.pop();
	                                           }},
	                                          {"Loa Game", [&]() { menuStack.push(createLoadMenu(*this, ".")); }},
	                                          {"Exit", [&]() { state = State::exit; }}};
	mainMenu = std::make_shared<menu::SelectionMenu>("Main Menu", mainMenuItems,
	                                                 std::ref(res.getMusic("waterflame-hexagon-force.ogg")));
	menuStack.push(mainMenu);
}

void Game::resetState()
{
	static SerializedState defaultState{0, firstRoom, {initialPosition}};
	loadState(defaultState);
}

void Game::saveState()
{
	SerializedState state{unlockedAreas, currentRoom->name, {player->movable.position}};

	// std::time_t now = time(nullptr);
	// char nowstr[100];
	// std::strftime(nowstr, sizeof(nowstr), "%d-%m-%y %H:%M:%S", std::localtime(&now));
	std::ofstream fout(std::string("Savegame.save") + std::to_string(SerializedState::compatibleVersion));
	if (!fout.is_open())
		std::cerr << "could not open save file" << std::endl;
	fout << state;
}

void Game::loadState(SerializedState state)
{
	currentRoom = std::make_unique<Room>(res.getRoom(state.currentRoomName));
	play_fade_in(currentRoom->music, repeat_forever, 500ms);
	player->movable.reposition(state.playerState.position);
	unlockedAreas = state.unlockedAreas;
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

static Position calcCameraPosition(const Player &player, const Room &room, const Renderer &renderer)
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
	while (state == State::running) {
		renderer.clear();

		// Game
		if (menuStack.empty()) {
			gameClock.resume();

			renderer.setCameraPosition(calcCameraPosition(*player, *currentRoom, renderer));
			auto now = gameClock.now();
			gameFrameDelta = now - lastGameFrameTime;

			// reset player velocity
			player->movable.mainLoopReset();

			// events
			playerHasMoved = false;
			gameEvents.dispatch();
			if (playerHasMoved)
				player->movable.startMoving();
			else
				player->movable.stopMoving();

			// gravity
			player->movable.applyGravity(gameFrameDelta);

			// Room
			// currentRoom->update()

			// Player
			player->movable.update(gameFrameDelta);
			resolveRoomCollision(*player, *currentRoom);

			// render
			renderer.render(*currentRoom, gameFrameDelta, renderOpts);
			renderer.render(*player, gameFrameDelta, renderOpts);

			lastGameFrameTime = now;

			// Menus
		} else {
			gameClock.pause();

			auto menu = menuStack.top();
			menu->dispatch();
			renderer.render(*menu, GameClock::duration(0), renderOpts);
		}

		renderer.swapBuffers();
	}

	music::fade_out(1s);
	music::block_until_stopped();
}

void Game::registerGameEvents()
{
	// quit
	gameEvents.on(SDL_QUIT, [this](const Event &) { state = State::exit; });
	gameEvents.onKeyDown(SDLK_ESCAPE, [this](const KeyboardEvent &) {
		std::vector<RawMenuItem> pauseMenuItems = {{"Stats", [&]() { /* TODO stats menu */ }},
		                                           {"Inventory", [&]() { /* TODO inventory menu */ }},
		                                           {"Resume", [&]() { menuStack.pop(); }},
		                                           {"Save",
		                                            [&]() {
			                                            saveState();
			                                            menuStack.pop();
		                                            }},
		                                           {"Main Menu", [&]() {
			                                            menuStack.pop();
			                                            menuStack.push(mainMenu);
			                                            mainMenu->playMusic();
		                                            }}};
		menuStack.push(
		    std::make_shared<menu::SelectionMenu>("Pause", pauseMenuItems, std::nullopt, [&]() { menuStack.pop(); }));
	});
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
	gameEvents.whileKeyHeld(SDL_SCANCODE_D, [this]() {
		player->movable.moveRight();
		playerHasMoved = true;
	});
	gameEvents.whileKeyHeld(SDL_SCANCODE_A, [this]() {
		player->movable.moveLeft();
		playerHasMoved = true;
	});
}
