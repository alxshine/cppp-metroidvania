#include "menu/InventoryMenu.hpp"

#include <algorithm>

using namespace menu;
using namespace game;
using namespace sdl;

InventoryMenu::InventoryMenu(std::set<Item> inventory, std::function<void()> escapeCallback) : items(inventory)
{
	eventHandler.onKeyDown(SDLK_ESCAPE, [=](const KeyboardEvent &) { escapeCallback(); });
}

void InventoryMenu::dispatch()
{
	eventHandler.dispatch();
}

void InventoryMenu::playMusic() {}

void InventoryMenu::render(const Renderer &renderer, GameClock::duration frameDelta, const sdl::RenderOptions &)
{
	const int margin_x = 30;
	const int padding_x = 10;
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

	// render items
	int x = margin_x;
	for (auto i : items) {
		const int size_x = 30;
		const int size_y = 30;
		Rectangle dest{x, y, size_x, size_y};

		// compute next grid position
		x += size_x + padding_x;
		// new line if necessary
		if (x + size_x + margin_x >= renderer.logicalW) {
			y += size_y + padding_y + 10; // TODO 10 is guess for text height
			x = margin_x;
		}

		renderer.render(i.animation.updateAnimation(frameDelta), dest);
		auto sanitizedItemName = i.name;
		std::replace(sanitizedItemName.begin(), sanitizedItemName.end(), '_', ' ');
		auto text = SDL::getInstance().generateText(*font, sanitizedItemName);
		dest.y += size_y + 1;
		dest.w = text.sourceRectangle.w;
		dest.h = text.sourceRectangle.h;
		renderer.render(text, dest);
	}
}
