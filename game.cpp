/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

// Using SDL, SDL_image, standard IO, and strings
#include <chrono>
#include <iostream>
#include <string>

#include "SDL.hpp"
#include "texture.hpp"

using namespace std;
using namespace std::chrono_literals;
using namespace sdl;
int main()
{
	try {
		auto sdl = SDL::getInstance();
		auto texture = sdl.loadTexture("sheet.png");
		auto sourceRect = Rectangle{0, 0, 122, 110};
		auto sprite = Sprite{texture, sourceRect};
		auto renderer = sdl.getRenderer();

		renderer->clear();
		renderer->render(sprite);
		renderer->swap();

		sdl.delay(2000ms);
	} catch (std::string &e) {
		cout << e << endl;
	}
}
