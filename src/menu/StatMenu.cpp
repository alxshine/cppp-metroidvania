#include "menu/StatMenu.hpp"

#include <algorithm>

using namespace menu;
using namespace game;
using namespace sdl;
using namespace std;

StatMenu::StatMenu(const Player &player, function<void()> escapeCallback)
    : level(player.getLevel()), xp(player.getXp()), hp(player.attackable.hp), maxHp(player.attackable.maxHp),
      threshold(player.levelThreshold)
{
	eventHandler.onKeyDown(SDLK_ESCAPE, [=](const KeyboardEvent &) { escapeCallback(); });
}

void StatMenu::dispatch()
{
	eventHandler.dispatch();
}

void StatMenu::playMusic() {}

int renderAt(SDL &sdl, const Renderer &renderer, Position position, const Font &font, string text)
{
	auto textImage = sdl.generateText(font, text);
	Rectangle dest{position.x, position.y, textImage.sourceRectangle.w, textImage.sourceRectangle.h};
	renderer.render(textImage, dest);
	return textImage.sourceRectangle.h;
}

void StatMenu::render(const Renderer &renderer, GameClock::duration, const sdl::RenderOptions &)
{
	const int margin_x = 30;
	const int padding_y = 10;
	int y = 10;

	// render background
	renderer.drawRectangle({0, 0, renderer.logicalW, renderer.logicalH},
	                       {0, 0, 0, static_cast<unsigned char>(opacity)});
	// render title
	auto target = title.sourceRectangle;
	target.x = margin_x;
	target.y = y;
	y += title.sourceRectangle.h + padding_y;
	renderer.render(title, target);

	int x = margin_x;
	y += padding_y;
	auto levelString = "Level: " + to_string(level);
	auto xpString = "Xp: " + to_string(xp) + "/" + to_string(threshold);
	auto hpString = "Hp: " + to_string(hp) + "/" + to_string(maxHp);

	auto &sdl = SDL::getInstance();
	y += padding_y + renderAt(sdl, renderer, {x, y}, *font, levelString);
	y += padding_y + renderAt(sdl, renderer, {x, y}, *font, xpString);
	y += padding_y + renderAt(sdl, renderer, {x, y}, *font, hpString);
}
