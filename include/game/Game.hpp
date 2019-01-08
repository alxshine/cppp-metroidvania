#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <algorithm>

#include "SDL.hpp"
#include "game/ResourceManager.hpp"

using namespace std::chrono_literals;

namespace game {

class Game {
  public:
	Game(std::string definitions, std::string assets, std::string first_room, Position player_position,
	     sdl::RenderOptions renderOpts = {false, false, false});

	void runMainLoop();
	void interact();

  private:
	ResourceManager res;
	std::unique_ptr<Room> currentRoom;
	std::unique_ptr<Entity> player;
	sdl::RenderOptions renderOpts;
	const sdl::Renderer &renderer = sdl::SDL::getInstance().getRenderer();

	bool running = true;
	sdl::GameClock gameClock;
	sdl::GameClock::time_point lastGameFrameTime;
	std::chrono::milliseconds gameFrameDelta = 0ms;
	sdl::EventHandler gameEvents;

	void registerGameEvents();
};

} // namespace game

#endif /* GAME_H */
