#include "menu/InventoryMenu.hpp"

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

void InventoryMenu::render(const Renderer &renderer, GameClock::duration frameDelta, const sdl::RenderOptions &options)
{
	const int margin_x = 30;
	const int padding_y = 10;
	int y = 30;

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
	for (auto &i : items) {
		// TODO reposition items according to grid
		// i.render(renderer, frameDelta, options);
		// renderer.render(i, frameDelta, options);
	}
}
