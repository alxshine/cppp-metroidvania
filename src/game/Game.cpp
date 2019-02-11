#include "game/Game.hpp"

#include "menu/InventoryMenu.hpp"
#include "menu/MapMenu.hpp"
#include "menu/MessageBox.hpp"

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
		                                           menuStack.pop();
		                                           resetState();
	                                           }},
	                                          {"Load Game", [&]() { menuStack.push(createLoadMenu(*this, ".")); }},
	                                          {"Exit", [&]() { state = State::exit; }}};
	mainMenu = std::make_shared<menu::SelectionMenu>("Main Menu", mainMenuItems,
	                                                 std::ref(res.getMusic("waterflame-hexagon-force.ogg")));
	menuStack.push(mainMenu);
}

void Game::resetState()
{
	static SerializedState defaultState{0, firstRoom, {"First"}, {}, {initialPosition, 1, 0}};
	loadState(defaultState);

	menuStack.push(std::make_shared<menu::MessageBox>([&]() { menuStack.pop(); }, "Welcome to our Metroidvania-like!",
	                                                  "Use WASD&SPACE for Movement",
	                                                  "and K&E for attack and interact"));
}

void Game::saveState()
{
	std::set<std::string> inventory;
	for (auto &i : player->inventory)
		inventory.insert(i.name);
	SerializedState state{unlockedAreas,
	                      currentRoom->name,
	                      player->visitedRooms,
	                      inventory,
	                      {player->movable.position, player->getLevel(), player->getXp()}};

	// std::time_t now = time(nullptr);
	// char nowstr[100];
	// std::strftime(nowstr, sizeof(nowstr), "%d-%m-%y %H:%M:%S", std::localtime(&now));
	std::ofstream fout(std::string("Savegame.save") + std::to_string(SerializedState::compatibleVersion));
	if (!fout.is_open())
		std::cerr << "could not open save file" << std::endl;
	else
		menuStack.push(std::make_shared<menu::MessageBox>([&]() { menuStack.pop(); }, "Game Saved!"));
	fout << state;
}

void Game::loadState(SerializedState state)
{
	std::set<Item> inventory;
	for (auto &i : state.inventory)
		inventory.insert(res.getItem(i));

	currentRoom = std::make_unique<Room>(res.getRoom(state.currentRoomName));
	player->visitedRooms = std::move(state.visitedRooms);
	player->inventory = inventory;
	if (std::any_of(player->inventory.cbegin(), player->inventory.cend(),
	                [](const Item &i) { return i.name == "JumpBoots"; }))
		player->movable.maxJumps = 2;
	else
		player->movable.maxJumps = 1;
	player->setLeveling(state.playerState.level, state.playerState.xp);

	play_fade_in(currentRoom->music, repeat_forever, 500ms);
	player->movable.reposition(state.playerState.position);
	player->attackable.reset();
	// TODO include attackable values in state
	// TODO include stats in state
	// TODO include inventory in state
}

void Game::interact()
{
	Rectangle playerHitbox = player->calcPositionedHitbox();

	// Find item to interact with
	for (auto &i : currentRoom->items) {
		if (intersects(playerHitbox, i.movable.calcPositionedHitbox()) && !i.pickedUp) {
			if (i.name == "Savepoint") {
				saveState();
				player->attackable.hp = player->attackable.maxHp;
			} else {
				// std::cout << "Player interacted with " << i.name << std::endl;
				player->inventory.insert(i);
				i.pickedUp = true;
				if (i.name == "JumpBoots")
					player->movable.maxJumps = 2; // TODO: this is ugly, but the fastest way to do it
				menuStack.push(std::make_unique<MessageBox>([&]() { menuStack.pop(); }, i.description));
			}
			return;
		}
	}

	// Find door to interact with
	for (auto door : currentRoom->doors) {
		if (intersects(playerHitbox, door.item.movable.calcPositionedHitbox())) {
			std::cout << "Player went to " << door.targetRoom << " through " << door.name << std::endl;

			// this conditions allows portals that don't reset room state
			if (currentRoom->name != door.targetRoom) {
				auto newRoom = std::make_unique<Room>(res.getRoom(door.targetRoom));
				// check if player has required key
				if (newRoom->gatingArea &&
				    !player->inventory.count(res.getItem(std::string("Key_") + std::to_string(newRoom->gatingArea)))) {
					menuStack.push(
					    std::make_shared<menu::MessageBox>([&]() { menuStack.pop(); }, "The door is locked."));
					return;
				}
				currentRoom = std::move(newRoom);
				player->visitedRooms.insert(currentRoom->name);
				// TODO if bosses already killed, destroy them
				// --> iterate mobs and kill any if they contain a number that is same as a key in inventory?
				play(currentRoom->music, repeat_forever);
			}

			auto newDoorIt = std::find_if(currentRoom->doors.cbegin(), currentRoom->doors.cend(),
			                              [door](Door d) { return d.name == door.targetDoorName; });
			if (newDoorIt == currentRoom->doors.cend()) {
				std::cerr << "No door named " << door.targetDoorName << " in room " << door.targetRoom << std::endl;
			} else {
				auto newPosition = newDoorIt->item.movable.getPosition();
				newPosition.x += player->movable.hitbox.w / 2;
				player->movable.reposition(newPosition);
				player->movable.setDirection(door.direction);
			}

			lastGameFrameTime = gameClock.now();
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
	if (p.y - player.movable.hitbox.h <= 0.6 * renderer.logicalH)
		camera.y = std::max(p.y - player.movable.hitbox.h - renderer.logicalH / 2, 0);
	return camera;
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

			// **************** SET ALIAS VARIABLES
			auto &mobs = currentRoom->mobs;
			std::vector<Rectangle> mobHitboxes;
			std::vector<Attackable *> mobAttackables;
			for (auto &mob : mobs) {
				mobHitboxes.push_back(mob.movable.calcPositionedHitbox());
				mobAttackables.push_back(&mob.attackable);
			}

			// **************** HANDLE THE PLAYER ****************
			// reset player velocity
			player->movable.mainLoopReset();

			// events
			playerHasMoved = false;
			gameEvents.dispatch();
			if (playerHasMoved)
				player->startMoving();
			else
				player->stopMoving();

			// gravity
			player->movable.applyGravity(gameFrameDelta);

			// Updates and collision
			player->update(gameFrameDelta);
			resolveRoomCollision(player->movable, currentRoom->collisionMap);

			// combat
			player->attackable.updateProjectiles(gameFrameDelta, mobHitboxes, mobAttackables, currentRoom->collisionMap,
			                                     player->getDamageMultiplier());
			player->attackable.launchProjectiles(player->movable.getPosition(), player->movable.getDirection());

			if (player->attackable.isAttacking()) {
				auto hitbox = player->getAttackHitbox();
				for (auto &m : mobs) {
					if (intersects(hitbox, m.movable.calcPositionedHitbox()))
						player->attackable.hit(m.attackable, player->getDamageMultiplier());
				}
			}

			// Room
			// currentRoom->update()

			// ******************* HANDLE THE MOBS ***************
			auto playerHitbox = player->movable.calcPositionedHitbox();
			for (auto &m : mobs) {
				// reset mob velocity
				m.movable.mainLoopReset();

				// AI
				m.performAiStep(currentRoom->collisionMap, playerHitbox);

				// gravity
				m.movable.applyGravity(gameFrameDelta);

				// Updates and collision
				m.movable.update(gameFrameDelta);
				resolveRoomCollision(m.movable, currentRoom->collisionMap);

				// combat
				m.attackable.update(gameFrameDelta);
				m.attackable.updateProjectiles(gameFrameDelta, playerHitbox, player->attackable,
				                               currentRoom->collisionMap);
				m.attackable.launchProjectiles(m.movable.getPosition(), m.movable.getDirection());

				if (m.attackable.isAttacking()) {
					if (intersects(m.attackable.getHitbox(m.movable.getPosition(), m.movable.getDirection().x < 0),
					               playerHitbox)) {
						m.attackable.hit(player->attackable);
					}
				}
			}

			// ******************* REMOVE UNNEEDED ***********
			auto emptyBefore = mobs.empty();
			auto end = remove_if(mobs.begin(), mobs.end(), [](Mob &m) { return m.attackable.done(); });
			for (auto it = end; it != mobs.end(); ++it)
				if (player->addXp(it->attackable.maxHp))
					menuStack.push(std::make_shared<menu::MessageBox>([&]() { menuStack.pop(); }, "Level up!"));
			mobs.erase(end, mobs.end());

			// spawn key items only if the mob list is empty

			if (!emptyBefore && mobs.empty()) {
				currentRoom->items.insert(currentRoom->items.end(), currentRoom->onClearItems.begin(),
				                          currentRoom->onClearItems.end());
				currentRoom->doors.insert(currentRoom->doors.end(), currentRoom->onClearDoors.begin(),
				                          currentRoom->onClearDoors.end());
			}

			if (player->attackable.done()) {
				std::vector<RawMenuItem> gameOverMenuItems = {
				    {"Main Menu",
				     [&]() {
					     menuStack.pop();
					     menuStack.push(mainMenu);
					     mainMenu->playMusic();
				     }},
				};
				// TODO play game over music? ... depending on win or loss?
				menuStack.push(
				    std::make_shared<SelectionMenu>("Game Over!", gameOverMenuItems, std::nullopt, [&]() {}));
			}

			// ******************* RENDERING *****************
			renderer.render(*currentRoom, gameFrameDelta, renderOpts);
			renderer.render(*player, gameFrameDelta, renderOpts);

			lastGameFrameTime = now;

		} else {
			// Menus
			gameClock.pause();

			// render game
			renderer.setCameraPosition(calcCameraPosition(*player, *currentRoom, renderer));
			renderer.render(*currentRoom, GameClock::duration(0), renderOpts);
			renderer.render(*player, GameClock::duration(0), renderOpts);

			// render menu
			renderer.resetCamera();
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
		std::vector<RawMenuItem> pauseMenuItems = {
		    {"Map",
		     [&]() {
			     menuStack.push(std::make_shared<MapMenu>(currentRoom->name, *player, res, [&]() { menuStack.pop(); }));
		     }},
		    {"Stats", [&]() { menuStack.push(std::make_shared<StatMenu>(*player, [&]() { menuStack.pop(); })); }},
		    {"Inventory",
		     [&]() {
			     menuStack.push(std::make_shared<InventoryMenu>(player->inventory, std::set{res.getMob("Shade")},
			                                               [&]() { menuStack.pop(); }));
		     }},
		    {"Resume", [&]() { menuStack.pop(); }},
		    {"Main Menu", [&]() {
			     menuStack.pop();
			     menuStack.push(mainMenu);
			     mainMenu->playMusic();
		     }}};
		menuStack.push(std::make_shared<menu::SelectionMenu>("Pause", pauseMenuItems, std::nullopt,
		                                                     [&]() { menuStack.pop(); }, 128));
	});
	// debug overlay
	gameEvents.onKeyDown(SDLK_c, [this](const KeyboardEvent &) {
		renderOpts.renderCollisionMap = !renderOpts.renderCollisionMap;
		renderOpts.renderEntityDrawRectangles = !renderOpts.renderEntityDrawRectangles;
		renderOpts.renderHitBoxes = !renderOpts.renderHitBoxes;
		renderOpts.renderHealthBars = !renderOpts.renderHealthBars;
	});

	// jump down from platforms
	gameEvents.whileKeyHeld(SDL_SCANCODE_S, [this]() { player->fall(); });
	// jump
	gameEvents.onKeyDown(SDLK_SPACE, [this](const KeyboardEvent &) { player->jump(); });

	// interaction
	gameEvents.onKeyDown(SDLK_e, [this](const KeyboardEvent &) { this->interact(); });

	// attack
	gameEvents.whileKeyHeld(SDL_SCANCODE_K, [this]() { player->attack(); });
	gameEvents.onKeyDown(SDLK_u, [this](const KeyboardEvent &) { player->shadeBlast(); });
	gameEvents.onKeyDown(SDLK_i, [this](const KeyboardEvent &) { player->singleSlam(); });
	gameEvents.onKeyDown(SDLK_o, [this](const KeyboardEvent &) { player->doubleSlam(); });

	// slide
	gameEvents.onKeyDown(SDLK_l, [this](const KeyboardEvent &) { player->slide(); });

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
		player->moveRight();
		playerHasMoved = true;
	});
	gameEvents.whileKeyHeld(SDL_SCANCODE_A, [this]() {
		player->moveLeft();
		playerHasMoved = true;
	});
}
