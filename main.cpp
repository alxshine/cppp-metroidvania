#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "SDL.hpp"
#include "game/ResourceManager.hpp"
#include "gameClock.hpp"
#include "menu.hpp"
#include "rendering.hpp"
#include "sdlException.hpp"

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
		ResourceManager man{"game_definitions", "assets"};

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

		auto mage = man.getMob("Mage");
		auto renderer = sdl.getRenderer();

		EventHandler events;
		events.on(SDLK_SPACE, [](const KeyboardEvent &e) {
			if (e.state == SDL_PRESSED)
				cout << "Space Pressed!" << endl;
		});

		GameClock clock;
		while (clock.now().time_since_epoch() < 5000ms) {
			renderer.clear();
			events.dispatch();
			renderer.render(mage, clock.now());
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

	} catch (SdlException &e) {
		cerr << e.what() << endl;
	} catch (const char *error) {
		cerr << error << endl;
	} catch (const string &error) {
		cerr << error << endl;
	}
}
