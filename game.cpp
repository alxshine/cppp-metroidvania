/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

// Using SDL, SDL_image, standard IO, and strings
#include <chrono>
#include <iostream>
#include <string>

#include "SDL.hpp"
#include "menu.hpp"
#include "sdlException.hpp"
#include "texture.hpp"

using namespace std;
using namespace std::chrono_literals;
using namespace sdl;
int main()
{
	try {
		SDL &sdl = SDL::getInstance();
		// auto texture = sdl.loadTexture("sheet.png");
		// Rectangle sourceRect{0, 0, 122, 110};
		// Sprite sprite{texture, sourceRect};
		auto renderer = sdl.getRenderer();

		auto titlefont = sdl.loadFont("assets/fonts/Countryside Personal Use.ttf", 60);
		auto buttonfont = sdl.loadFont("assets/fonts/Countryside Personal Use.ttf", 30);

		auto title = sdl.generateText(titlefont, "Main Menu");
		Menu menu{title, {100, 0, 0, 0}, renderer};

		auto text1 = sdl.generateText(buttonfont, "Button 1");
		menu.addItem({text1, [] {}, {100, 100, 100, 0}});
		auto text2 = sdl.generateText(buttonfont, "Button 2");
		menu.addItem({text2, [] {}});
		auto text3 = sdl.generateText(buttonfont, "Button with longer label");
		menu.addItem({text3, [] {}});

		renderer->clear();
		menu.render();
		// renderer->render(text);
		renderer->swapBuffers();

		sdl.delay(2000ms);
	} catch (SdlException &e) {
		cout << e.what() << endl;
	}
}
