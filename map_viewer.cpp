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

	EventHandler events;
	bool run = true;
	events.on(SDL_QUIT, [&run](const Event &) { run = false; });
	Position cam{0, 0};
	events.whileKeyHeld(SDL_SCANCODE_W, [&cam]() { cam.y -= 5; });
	events.whileKeyHeld(SDL_SCANCODE_A, [&cam]() { cam.x -= 5; });
	events.whileKeyHeld(SDL_SCANCODE_S, [&cam]() { cam.y += 5; });
	events.whileKeyHeld(SDL_SCANCODE_D, [&cam]() { cam.x += 5; });
	events.onKeyDown(SDLK_r, [&cam](const KeyboardEvent &) { cam = {0, 0}; });

	auto renderer(SDL::getInstance().getRenderer());

	bool error = false;
	int i = 0;
	while (run) {
		i++;
		renderer.clear();
		renderer.setCameraPosition(cam);
		events.dispatch();

		if (i == 60)
			try {
				i = 0;
				error = true;
				res = {"../game_definitions", "../assets"};
				error = false;
			} catch (SdlException &e) {
				cerr << e.what() << endl;
			} catch (GameException &e) {
				cerr << e.what() << endl;
			} catch (std::runtime_error &e) {
				cerr << e.what() << endl;
			} catch (const string &e) {
				cerr << e << endl;
			}

		if (!error) {
			Room room(res.getRoom(roomName));
			renderer.render(room, GameClock::duration(0), {true, false, false});
		}

		renderer.swapBuffers();
	}
}
