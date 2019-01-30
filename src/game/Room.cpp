#include "game/Room.hpp"

using namespace game;
using namespace std;

MapRoom::MapRoom(Rectangle drawBox, string name, set<string> connectedRooms, bool hasSavepoint)
    : boundingBox(move(drawBox)), name(move(name)), connectedRooms(move(connectedRooms)), hasSavepoint(hasSavepoint)
{
}

Rectangle Room::Tile::render(const sdl::Renderer &renderer, const Position targetPosition) const
{
	Rectangle targetRect{targetPosition.x, targetPosition.y, tileSize.w, tileSize.h};
	renderer.render(sprite, targetRect);
	return targetRect;
}

inline Rectangle getMapSize(Position location, const CollisionMap &collisionMap)
{
	return {location.x, location.y, static_cast<int>(collisionMap[0].size()), static_cast<int>(collisionMap.size())};
}

inline set<string> getConnectedNames(const vector<Door> &doors)
{
	set<string> ret;
	for (auto &d : doors)
		ret.insert(d.targetRoom);
	return ret;
}

inline bool hasSavePoint(const vector<Item> &items)
{
	for (auto &i : items)
		if (i.name == "Savepoint")
			return true;
	return false;
}

Room::Room(string name, const sdl::Texture &background, const sdl::Music &music, Position location, int gatingArea,
           Room::Layout layout, CollisionMap collisionMap, vector<Mob> mobs, vector<Item> items,
           vector<Item> onClearItems, vector<Door> doors)
    : name(name), background(background), music(music), location(location), gatingArea(gatingArea), layout(layout),
      sizeInPixels(
          {0, 0, static_cast<int>(layout[0][0].size()) * tileSize.w, static_cast<int>(layout[0].size()) * tileSize.h}),
      collisionMap(collisionMap), mobs(mobs), items(items), onClearItems(onClearItems), doors((doors)),
      mapVersion(getMapSize(location, collisionMap), name, getConnectedNames(doors), hasSavePoint(items))
{
}

Room::Room(const Room &rhs) noexcept
    : name(rhs.name), background(rhs.background), music(rhs.music), location(rhs.location), gatingArea(rhs.gatingArea),
      layout(rhs.layout), sizeInPixels(rhs.sizeInPixels), collisionMap(rhs.collisionMap), mapVersion(rhs.mapVersion),
      mobs(rhs.mobs), items(rhs.items), onClearItems(rhs.onClearItems), doors(rhs.doors)
{
}

void Room::render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta, const sdl::RenderOptions &options)
{
	// TODO: parallax
	renderer.render(background);

	for (auto &layer : layout) {
		auto locationY = 0;
		for (auto &row : layer) {
			auto locationX = 0;
			for (auto &tile : row) {
				tile.render(renderer, {locationX, locationY});
				locationX += tileSize.w;
			}
			locationY += tileSize.h;
		}
	}

	if (options.renderCollisionMap) {
		auto locationY = 0;
		for (auto &row : collisionMap) {
			auto locationX = 0;
			for (auto &tile : row) {
				Rectangle targetRect{locationX, locationY, tileSize.w, tileSize.h};
				sdl::Color color{0, 0, 0, 100};
				switch (tile) {
				case Collision::TopOnly:
					color.g = 255;
					break;
				case Collision::Full:
					color.r = 255;
					break;
				case Collision::None:
					break;
				}
				renderer.drawRectangle(targetRect, color);
				locationX += tileSize.w;
			}
			locationY += tileSize.h;
		}
	}

	for (auto i : doors)
		renderer.render(i.item, frameDelta, options);
	for (auto &i : mobs)
		renderer.render(i, frameDelta, options);
	for (auto &i : items)
		if (!i.pickedUp) // TODO remove from room?
			renderer.render(i, frameDelta, options);
}

Room::~Room() {}
