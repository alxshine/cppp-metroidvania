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

		// vector<Rectangle> frames;
		// frames.push_back(Rectangle{0, 0, 122, 110});
		// frames.push_back(Rectangle{122, 0, 122, 110});
		// frames.push_back(Rectangle{244, 0, 122, 110});
		// frames.push_back(Rectangle{366, 0, 122, 110});
		// frames.push_back(Rectangle{0, 110, 122, 110});
		// frames.push_back(Rectangle{122, 110, 122, 110});
		// frames.push_back(Rectangle{244, 110, 122, 110});
		// frames.push_back(Rectangle{366, 110, 122, 110});
		// frames.push_back(Rectangle{0, 110, 122, 110});

		// Animation a{*texture, frames, 50ms};

		// auto mage = res.getMob("Mage");
		auto room = res.getRoom("First");
		auto &renderer = sdl.getRenderer();

		play(room.music, repeat_forever);

		auto &sound = res.getSound("hey.wav");

		EventHandler events;
		events.on(SDLK_SPACE, [&](const KeyboardEvent &e) {
			if (e.state == SDL_PRESSED) {
				cout << "Space Pressed!" << endl;
				play(sound);
			}
		});

		GameClock clock;
		bool running = true;
		events.on(SDLK_ESCAPE, [&](const KeyboardEvent &e) {
			if (e.state == SDL_PRESSED)
				running = false;
		});
		while (running) {
			renderer.clear();
			events.dispatch();
			renderer.render(room, clock.now());
			renderer.swapBuffers();
		}

		// auto titlefont = sdl.loadFont("assets/fonts/Countryside Personal Use.ttf", 60);
		// auto buttonfont = sdl.loadFont("assets/fonts/Countryside Personal Use.ttf", 30);

		// auto title = sdl.generateText(*titlefont, "Main Menu");
		// Menu menu{title, {100, 0, 0, 0}, renderer};

		// auto text1 = sdl.generateText(*buttonfont, "Button 1");
		// menu.addItem({text1, [] {}, {100, 100, 100, 0}});
		// auto text2 = sdl.generateText(*buttonfont, "Button 2");
		// menu.addItem({text2, [] {}});
		// auto text3 = sdl.generateText(*buttonfont, "Button with longer label");
		// menu.addItem({text3, [] {}});

		// renderer->render(menu);
		// renderer->swapBuffers();
		// sdl.delay(2000ms);

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
