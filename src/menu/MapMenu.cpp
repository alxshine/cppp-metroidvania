#include "menu/MapMenu.hpp"

using namespace menu;
using namespace game;
using namespace sdl;
using namespace std;

MapMenu::MapMenu(const Player &player, const ResourceManager &resourceManager, function<void()> escapeCallback)
{
	eventHandler.onKeyDown(SDLK_ESCAPE, [=](const KeyboardEvent &) { escapeCallback(); });

	toRender = resourceManager.getMapRooms(player.visitedRooms);
}

void MapMenu::render(const Renderer &renderer, GameClock::duration frameDelta, const RenderOptions &options)
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
	int y_after_title = y;
	renderer.render(title, target);

	cout << toRender.size() << endl;
}

void MapMenu::dispatch()
{
	eventHandler.dispatch();
}

void MapMenu::playMusic() {}
