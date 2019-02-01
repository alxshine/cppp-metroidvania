#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "SDL.hpp"
#include "game/Game.hpp"

using namespace std;
using namespace std::chrono_literals;
using namespace sdl;
using namespace game;

int main(int, char **argv)
{
	std::string roomName = argv[1];
	ResourceManager res{"../game_definitions", "../assets"};

	// allow reloading assets
	EventHandler events;
	bool run = true;
	events.on(SDL_QUIT, [&run](const Event &) { run = false; });
	events.onKeyDown(SDLK_r, [&res](const KeyboardEvent &) {
		try {
			res = {"../game_definitions", "../assets"};
		} catch (SdlException &e) {
			cerr << e.what() << endl;
		} catch (GameException &e) {
			cerr << e.what() << endl;
		} catch (const string &e) {
			cerr << e << endl;
		}
	});
	Position cam{0,0};
	events.whileKeyHeld(SDL_SCANCODE_W, [&cam]() { cam.y -= 5; });
	events.whileKeyHeld(SDL_SCANCODE_A, [&cam]() { cam.x -= 5; });
	events.whileKeyHeld(SDL_SCANCODE_S, [&cam]() { cam.y += 5; });
	events.whileKeyHeld(SDL_SCANCODE_D, [&cam]() { cam.x += 5; });

	auto renderer(SDL::getInstance().getRenderer());

	while (run) {
		renderer.clear();
		renderer.setCameraPosition(cam);
		events.dispatch();

		Room room(res.getRoom(roomName));
		renderer.render(room, GameClock::duration(0));

		renderer.swapBuffers();
	}
}
