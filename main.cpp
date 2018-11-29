#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "SDL.hpp"
#include "game/ResourceManager.hpp"

using namespace std;
using namespace std::chrono_literals;
using namespace sdl;
using namespace game;

void printTimePoint(string name, const GameClock::time_point &t)
{
	cout << name + ": " << std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch()).count() << endl;
}

int main()
{
	try {
		SDL &sdl = SDL::getInstance();
		ResourceManager res{"game_definitions", "assets"};

		// auto mage = res.getMob("Mage");
		auto room = res.getRoom("First");
		auto player = res.makePlayer();
		player->movable.reposition({50, 170});
		auto &renderer = sdl.getRenderer();

		play(room.music, repeat_forever);

		// auto &sound = res.getSound("hey.wav");

		{
			GameClock clock;
			GameClock::time_point last_frame_time;
			std::chrono::milliseconds frame_delta = 0ms;
			bool running = true;
			EventHandler events;
			sdl::RenderOptions options{true, true, true};

			// Key bindings
			{
				events.on(SDL_QUIT, [&](const Event &) { running = false; });
				events.onKeyDown(SDLK_ESCAPE, [&running](const KeyboardEvent &) { running = false; });
				events.onKeyDown(SDLK_c, [&options](const KeyboardEvent &) {
					options.renderCollisionMap = !options.renderCollisionMap;
					options.renderEntityDrawRectangles = !options.renderEntityDrawRectangles;
					options.renderHitBoxes = !options.renderHitBoxes;
				});

				events.whileKeyHeld(SDL_SCANCODE_D, [&player, &frame_delta]() {
					player->movable.move({1, 0}, frame_delta);
				});
				events.whileKeyHeld(SDL_SCANCODE_A, [&player, &frame_delta]() {
					player->movable.move({-1, 0}, frame_delta);
				});
			}

			while (running) {
				renderer.clear();
				auto now = clock.now();
				frame_delta = now - last_frame_time;

				events.dispatch();

				// UI

				// Room
				renderer.render(room, now, options);

				// Player
				player->movable.update();
				renderer.render(*player, now, options);

				last_frame_time = now;
				renderer.swapBuffers();
			}
		}

		music::fade_out(1s);
		music::block_until_stopped();

	} catch (SdlException &e) {
		cerr << e.what() << endl;
	} catch (GameException &e) {
		cerr << e.what() << endl;
	} catch (const string &e) {
		cerr << e << endl;
	}
}
