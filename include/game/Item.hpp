#ifndef ITEM_H
#define ITEM_H

#include "Movable.hpp"
#include "constants.hpp"
#include "gamedef/entity_definitions.hpp"

namespace game {

class Item : public sdl::Renderable {
  public:
	Item(std::string name, std::string description, Rectangle hitbox, Rectangle renderSize, sdl::Animation animation);
	Item(const Item &rhs);
	virtual ~Item();

	void render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
	            const sdl::RenderOptions &options = {}) override;

	std::string name;
	std::string description;
	Movable movable;
	bool pickedUp = false;
	sdl::Animation animation;

  private:
	Rectangle calcRenderTarget() const;
	Rectangle renderSize;
};

inline bool operator<(const Item &a, const Item &b)
{
	return a.name < b.name;
}

class Door {
  public:
	Door(const std::string name, Item actualDoor, Direction direction, const std::string targetRoom,
	     const std::string targetDoorName);
	Door(const Door &rhs);

	std::string name;
	Item item;

	Direction direction;
	std::string targetRoom;
	std::string targetDoorName;
};

} // namespace game

#endif /* ITEM_H */
