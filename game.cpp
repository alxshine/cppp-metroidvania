/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

// Using SDL, SDL_image, standard IO, and strings
#include <chrono>
#include <iostream>
#include <string>

#include "SDL.hpp"
#include "gameClock.hpp"
#include "sdlException.hpp"
#include "texture.hpp"

using namespace std;
using namespace std::chrono_literals;
using namespace sdl;

void printTimePoint(string name, const GameClock::time_point &t){
		cout << name +": " << std::chrono::duration_cast<std::chrono::milliseconds>(t.time_since_epoch()).count()
			 << endl;
}

int main()
{
	try {
		SDL &sdl = SDL::getInstance();
		// auto texture = sdl.loadTexture("sheet.png");
		// Rectangle sourceRect{0, 0, 122, 110};
		// Sprite sprite{texture, sourceRect};
		//auto renderer = sdl.getRenderer();

		//auto font = sdl.loadFont("assets/fonts/Countryside Personal Use.ttf", 60);
		//auto text = sdl.generateText(font, "Hello World!");

		//renderer->clear();
		//renderer->render(text);
		//renderer->swapBuffers();

		GameClock clock;

		printTimePoint("now", clock.now());
		cout << "Delaying 500ms" << endl;
		sdl.delay(500ms);
		printTimePoint("now", clock.now());
		cout << "Pausing" << endl;
		clock.pause();
		cout << "Delaying 200ms" << endl;
		sdl.delay(200ms);
		printTimePoint("now", clock.now());
		cout << "Resuming" << endl;
		clock.resume();
		cout << "Delaying 100ms" << endl;
		sdl.delay(100ms);
		printTimePoint("now", clock.now());

	} catch (SdlException &e) {
		cout << e.what() << endl;
	}
}
