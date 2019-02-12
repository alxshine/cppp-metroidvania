#include "menu/StatMenu.hpp"

#include <algorithm>

using namespace menu;
using namespace game;
using namespace sdl;
using namespace std;

StatMenu::StatMenu(const Player &player, function<void()> escapeCallback)
    : level(player.getLevel()), xp(player.getXp()), hp(player.attackable.hp), maxHp(player.attackable.maxHp),
      threshold(player.levelThreshold), multiplier(player.getDamageMultiplier()),
      dmg1(player.attackable.attacks[0].damage * multiplier), dmg2(player.attackable.attacks[1].damage * multiplier),
      dmg3(player.attackable.attacks[2].damage * multiplier)
{
	eventHandler.onKeyDown(SDLK_ESCAPE, [=](const KeyboardEvent &) { escapeCallback(); });

	if (std::any_of(player.inventory.cbegin(), player.inventory.cend(), [](const Item &i) { return i.name == "ShadePotion"; }))
		dmg4 = player.attackable.attacks[3].projectileBlueprint->damage * multiplier;
	if (std::any_of(player.inventory.cbegin(), player.inventory.cend(), [](const Item &i) { return i.name == "GolemPotion"; }))
		dmg5 = player.attackable.attacks[4].damage * multiplier;
	if (std::any_of(player.inventory.cbegin(), player.inventory.cend(), [](const Item &i) { return i.name == "YetiPotion"; }))
		dmg6 = player.attackable.attacks[5].damage * multiplier;
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
	int y_after_title = y;
	auto levelString = "Level: " + to_string(level);
	auto xpString = "Xp: " + to_string(xp) + "/" + to_string(level * threshold);
	auto hpString = "Hp: " + to_string(hp) + "/" + to_string(maxHp);
	auto &sdl = SDL::getInstance();
	y += padding_y + renderAt(sdl, renderer, {x, y}, *font, levelString);
	y += padding_y + renderAt(sdl, renderer, {x, y}, *font, xpString);
	y += padding_y + renderAt(sdl, renderer, {x, y}, *font, hpString);

	auto dmg1String = "Dmg: " + to_string(dmg1);
	auto dmg2String = "Combo 1: " + to_string(dmg2);
	auto dmg3String = "Combo 2: " + to_string(dmg3);
	auto dmg4String = "Shade: " + to_string(dmg4);
	auto dmg5String = "Golem: " + to_string(dmg5);
	auto dmg6String = "Yeti: " + to_string(dmg6);
	x += 160;
	y = y_after_title;
	y += padding_y + renderAt(sdl, renderer, {x, y}, *font, dmg1String);
	y += padding_y + renderAt(sdl, renderer, {x, y}, *font, dmg2String);
	y += padding_y + renderAt(sdl, renderer, {x, y}, *font, dmg3String);
	if (dmg4)
		y += padding_y + renderAt(sdl, renderer, {x, y}, *font, dmg4String);
	if (dmg5)
		y += padding_y + renderAt(sdl, renderer, {x, y}, *font, dmg5String);
	if (dmg6)
		y += padding_y + renderAt(sdl, renderer, {x, y}, *font, dmg6String);
}
