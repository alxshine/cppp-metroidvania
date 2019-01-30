#include "menu/MapMenu.hpp"

using namespace menu;
using namespace game;
using namespace sdl;
using namespace std;

MapMenu::MapMenu(const Player &player, const ResourceManager &resourceManager, function<void()> escapeCallback)
{
	eventHandler.onKeyDown(SDLK_ESCAPE, [=](const KeyboardEvent &) { escapeCallback(); });

	toRender = resourceManager.getMapRooms(player.visitedRooms);
	for (auto &pair : toRender) {
		cout << pair.second.boundingBox << endl;
	}

	auto start = toRender.cbegin();
	auto end = toRender.cend();
	auto leftest = min_element(
	    start, end, [](auto &lhs, auto &rhs) { return lhs.second.boundingBox.x < rhs.second.boundingBox.x; });
	auto rightest = max_element(start, end, [](auto &lhs, auto &rhs) {
		return lhs.second.boundingBox.x + lhs.second.boundingBox.w <
		       rhs.second.boundingBox.x + rhs.second.boundingBox.y;
	});
	auto highest = min_element(
	    start, end, [](auto &lhs, auto &rhs) { return lhs.second.boundingBox.y < rhs.second.boundingBox.y; });
	auto lowest = max_element(start, end, [](auto &lhs, auto &rhs) {
		return lhs.second.boundingBox.y + lhs.second.boundingBox.h <
		       rhs.second.boundingBox.y + rhs.second.boundingBox.h;
	});

	minX = leftest->second.boundingBox.x;
	maxX = rightest->second.boundingBox.x + rightest->second.boundingBox.w;
	minY = highest->second.boundingBox.y;
	maxY = lowest->second.boundingBox.y + lowest->second.boundingBox.h;
	cout << "minX: " << minX << ", maxX: " << maxX << ", minY: " << minY << ", maxY: " << maxY << endl;
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
	renderer.render(title, target);

	int y_after_title = y + padding_y;

	auto renderWidth = renderer.logicalW - 2 * margin_x;
	auto renderHeight = renderer.logicalH - y_after_title - padding_y;
	auto distX = maxX - minX;
	auto distY = maxY - minY;

	// render map versions of rooms
	for (const auto &pair : toRender) {
		const auto &mr = pair.second;

		auto boundingBox = mr.boundingBox;
		// offset by minX and minY
		boundingBox.x -= minX;
		boundingBox.y -= minY;
		// scale to render size
		boundingBox.x = boundingBox.x * renderWidth / distX;
		boundingBox.y = boundingBox.y * renderHeight / distY;
		boundingBox.w = boundingBox.w * renderWidth / distX;
		boundingBox.h = boundingBox.h * renderHeight / distY;
		// offset by margin x and y_after_title
		boundingBox.x += margin_x;
		boundingBox.y += y_after_title;

		if (mr.hasSavepoint)
			renderer.drawRectangle(boundingBox, {255, 255, 255, 128});
		else
			renderer.drawRectangle(boundingBox, {0, 0, 255, 128});
	}
}

void MapMenu::dispatch()
{
	eventHandler.dispatch();
}

void MapMenu::playMusic() {}
