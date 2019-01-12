#include "menu/MessageBox.hpp"

using namespace menu;
using namespace sdl;

MessageBox::MessageBox(std::function<void()> action, std::string line1, std::string line2, std::string line3)
    : line1(SDL::getInstance().generateText(*font, line1))
{
	if (line2 != "") {
		lineCount++;
		this->line2 = SDL::getInstance().generateText(*font, line2);
	}
	if (line3 != "") {
		lineCount++;
		this->line3 = SDL::getInstance().generateText(*font, line3);
	}
	eventHandler.onKeyDown(SDLK_RETURN, [=](const KeyboardEvent &) { action(); });
}

void MessageBox::dispatch()
{
	eventHandler.dispatch();
}

void MessageBox::render(const sdl::Renderer &renderer, sdl::GameClock::duration, const sdl::RenderOptions &)
{
	const int lineheight = line1.sourceRectangle.h;
	const int margin_x = 15;
	const int padding_y = 5;

	int y = renderer.logicalH - padding_y - lineCount * (lineheight + padding_y);
	renderer.drawRectangle({0, y, renderer.logicalW, renderer.logicalH}, {0, 0, 0, (unsigned char)opacity});

	y += padding_y;
	auto target = line1.sourceRectangle;
	target.x = margin_x;
	target.y = y;
	renderer.render(line1, target);

	if (lineCount >= 2) {
		y += padding_y + lineheight;
		auto target = line2.sourceRectangle;
		target.x = margin_x;
		target.y = y;
		renderer.render(line2, target);
	}

	if (lineCount >= 3) {
		y += padding_y + lineheight;
		auto target = line3.sourceRectangle;
		target.x = margin_x;
		target.y = y;
		renderer.render(line3, target);
	}
}
