#ifndef GAME_H
#define GAME_H

#include <algorithm>
#include <iostream>

#include "SDL.hpp"
#include "game/ResourceManager.hpp"
#include "game/physics.hpp"

using namespace std::chrono_literals;

namespace game {

class Game {
  public:
	Game(std::string definitions, std::string assets, std::string first_room, Position player_position,
	     sdl::RenderOptions renderOpts = {false, false, false});

	void initialize();
	void runMainLoop();
	void interact();
	sdl::GameClock::time_point lastGameFrameTime;

  private:
	ResourceManager res;
	std::unique_ptr<Room> currentRoom;
	std::unique_ptr<Player> player;
	sdl::RenderOptions renderOpts;
	const sdl::Renderer &renderer = sdl::SDL::getInstance().getRenderer();

	bool running = true;
	sdl::GameClock gameClock;
	std::chrono::milliseconds gameFrameDelta = 0ms;
	sdl::EventHandler gameEvents;

	void registerGameEvents();
};

} // namespace game

#endif /* GAME_H */
