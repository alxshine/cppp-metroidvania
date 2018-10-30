/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

// Using SDL, SDL_image, standard IO, and strings
#include <chrono>
#include <iostream>
#include <string>

#include "SDL.hpp"
#include "sdlException.hpp"
#include "texture.hpp"

using namespace std;
using namespace std::chrono_literals;
using namespace sdl;
int main()
{
	try {
		SDL& sdl = SDL::getInstance();
		// auto texture = sdl.loadTexture("sheet.png");
		// Rectangle sourceRect{0, 0, 122, 110};
		// Sprite sprite{texture, sourceRect};
		auto renderer = sdl.getRenderer();

		auto font = sdl.loadFont("assets/fonts/Countryside Personal Use.ttf", 60);
		auto text = sdl.generateText(font, "Hello World!");

		renderer->clear();
		renderer->render(text);
		renderer->swapBuffers();

		sdl.delay(2000ms);
	} catch (SdlException &e) {
		cout << e.what() << endl;
	}
}
