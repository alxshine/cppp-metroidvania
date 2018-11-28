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
		player->movable.move({5, 17});
		auto &renderer = sdl.getRenderer();

		play(room.music, repeat_forever);

		// auto &sound = res.getSound("hey.wav");

		{
			GameClock clock;
			bool running = true;
			EventHandler events;

			events.on(SDL_QUIT, [&](const Event &) { running = false; });
			events.onKeyDown(SDLK_ESCAPE, [&running](const KeyboardEvent &) { running = false; });

			sdl::RenderOptions options{true, true, true};
			events.onKeyDown(SDLK_c, [&options](const KeyboardEvent &) {
				options.renderCollisionMap = !options.renderCollisionMap;
			});

			// TODO: base movement on HOLDING key, not pressing - repeat press events are too slow to start for movement I think --> extend EventHandler::whileKeyHeld .
			// TODO: make movement speed frame-rate independent
			// TODO: make movement not be based on tiles. this mostly means changing Player::render().
			events.onKeyDown(SDLK_d, [&player](const KeyboardEvent &) {
				player->movable.move({player->movable.getPosition().x + 1, player->movable.getPosition().y});
			});
			events.onKeyDown(SDLK_a, [&player](const KeyboardEvent &) {
				player->movable.move({player->movable.getPosition().x - 1, player->movable.getPosition().y});
			});

			while (running) {
				renderer.clear();
				events.dispatch();
				auto t = clock.now();

				renderer.render(room, t, options);

				player->movable.update();
				renderer.render(*player, t, options);

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
