#include "menu/MapMenu.hpp"

using namespace menu;
using namespace game;
using namespace sdl;
using namespace std;

MapMenu::MapMenu(string currentRoom, const Player &player, const ResourceManager &resourceManager,
                 function<void()> escapeCallback)
    : currentRoom(move(currentRoom))
{
	eventHandler.onKeyDown(SDLK_ESCAPE, [=](const KeyboardEvent &) { escapeCallback(); });

	toRender = resourceManager.getMapRooms(player.visitedRooms);

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
}

inline Point getCenter(Rectangle r)
{
	return {r.x + r.w / 2, r.y + r.h / 2};
}

void MapMenu::render(const Renderer &renderer, GameClock::duration, const RenderOptions &)
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

	int y_after_title = y + padding_y;

	auto renderWidth = renderer.logicalW - 2 * margin_x;
	auto renderHeight = renderer.logicalH - y_after_title - padding_y;
	auto distX = maxX - minX;
	auto distY = maxY - minY;

	map<string, MapRoom> scaledMapRooms;

	// scale rooms
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

		scaledMapRooms.emplace(mr.name, MapRoom{boundingBox, mr.name, mr.connectedRooms, mr.hasSavepoint});
	}

	// draw connections via doors
	for (const auto &pair : scaledMapRooms) {
		const auto &mr = pair.second;
		for (auto s : mr.connectedRooms) {
			const auto &other = scaledMapRooms.at(s);
			renderer.drawLine(getCenter(mr.boundingBox), getCenter(other.boundingBox), {255, 255, 0, 255});
		}
	}

	// draw rooms
	for (const auto &pair : scaledMapRooms) {
		const auto &mr = pair.second;
		if (mr.name == currentRoom)
			renderer.drawRectangle(mr.boundingBox, {0, 255, 0, 255});
		else if (mr.hasSavepoint)
			renderer.drawRectangle(mr.boundingBox, {255, 255, 255, 255});
		else
			renderer.drawRectangle(mr.boundingBox, {255, 0, 0, 255});
	}
}

void MapMenu::dispatch()
{
	eventHandler.dispatch();
}

void MapMenu::playMusic() {}
