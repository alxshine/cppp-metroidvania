#ifndef GAME_H
#define GAME_H

#include <algorithm>
#include <iostream>
#include <stack>

#include "SDL.hpp"
#include "game/ResourceManager.hpp"
#include "game/physics.hpp"
#include "menu/SelectionMenu.hpp"

using namespace std::chrono_literals;

namespace game {

class Game {
	enum class State { running, exit };

  public:
	Game(std::string definitions, std::string assets, std::string first_room, Position player_position,
	     sdl::RenderOptions renderOpts = {false, false, false});

	void initialize();
	void resetState();
	void saveState();
	void loadState(std::string name);

	void runMainLoop();
	void interact();
	sdl::GameClock::time_point lastGameFrameTime;

  private:
	std::shared_ptr<menu::Menu> mainMenu;
	std::stack<std::shared_ptr<menu::Menu>> menuStack;
	ResourceManager res;
	const std::string firstRoom;
	const Position initialPosition;
	std::unique_ptr<Room> currentRoom;
	std::unique_ptr<Player> player;
	sdl::RenderOptions renderOpts;
	const sdl::Renderer &renderer = sdl::SDL::getInstance().getRenderer();

	State state = State::running;
	bool playerHasMoved = false;
	sdl::GameClock gameClock;
	std::chrono::milliseconds gameFrameDelta = 0ms;
	sdl::EventHandler gameEvents;

	void registerGameEvents();
};

} // namespace game

#endif /* GAME_H */
