#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "SDL.hpp"
<<<<<<< HEAD
#include "animation.hpp"
#include "gameClock.hpp"
||||||| merged common ancestors
=======
#include "menu.hpp"
>>>>>>> menu
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
		auto texture = sdl.loadTexture("sheet.png");

		vector<Rectangle> frames;
		frames.push_back(Rectangle{0, 0, 122, 110});
		frames.push_back(Rectangle{122, 0, 122, 110});
		frames.push_back(Rectangle{244, 0, 122, 110});
		frames.push_back(Rectangle{366, 0, 122, 110});
		frames.push_back(Rectangle{0, 110, 122, 110});
		frames.push_back(Rectangle{122, 110, 122, 110});
		frames.push_back(Rectangle{244, 110, 122, 110});
		frames.push_back(Rectangle{366, 110, 122, 110});
		frames.push_back(Rectangle{0, 110, 122, 110});

		Animation a{texture, frames, 50ms};

		auto renderer = sdl.getRenderer();

		GameClock clock;
		while (clock.now().time_since_epoch() < 5000ms) {
			auto sprite = a.getAnimationFrame(clock.now());
			renderer->clear();
			renderer->render(sprite);
			renderer->swapBuffers();
		}

	} catch (SdlException &e) {
		cout << e.what() << endl;
	}
}
