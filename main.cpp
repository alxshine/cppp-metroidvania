#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "SDL.hpp"
#include "gameClock.hpp"
#include "menu.hpp"
#include "sdlException.hpp"
#include "rendering.hpp"

using namespace std;
using namespace std::chrono_literals;
using namespace sdl;

void printTimePoint(string name, const GameClock::time_point &t)
{
	cout << name + ": " << std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch()).count() << endl;
}

int main()
{
	try {
		SDL &sdl = SDL::getInstance();
		// auto texture = sdl.loadTexture("sheet.png");
        //
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
        //
		// Animation a{texture, frames, 50ms};

		auto renderer = sdl.getRenderer();

		// GameClock clock;
		// while (clock.now().time_since_epoch() < 5000ms) {
		// 	auto sprite = a.getAnimationFrame(clock.now());
		// 	renderer->clear();
		// 	renderer->render(sprite);
		// 	renderer->swapBuffers();
		// }


		auto titlefont = sdl.loadFont("assets/fonts/Countryside Personal Use.ttf", 60);
		auto buttonfont = sdl.loadFont("assets/fonts/Countryside Personal Use.ttf", 30);

		auto title = sdl.generateText(*titlefont, "Main Menu");
		Menu menu{title, {100, 0, 0, 0}, renderer};

		auto text1 = sdl.generateText(*buttonfont, "Button 1");
		menu.addItem({text1, [] {}, {100, 100, 100, 0}});
		auto text2 = sdl.generateText(*buttonfont, "Button 2");
		menu.addItem({text2, [] {}});
		auto text3 = sdl.generateText(*buttonfont, "Button with longer label");
		menu.addItem({text3, [] {}});

		renderer->render(menu);
		renderer->swapBuffers();
		sdl.delay(2000ms);

	} catch (SdlException &e) {
		cout << e.what() << endl;
	}
}
