#ifndef ITEM_H
#define ITEM_H

#include "gamedef/entity_definitions.hpp"
#include "constants.hpp"
#include "Movable.hpp"

namespace game {

class Item : public sdl::Renderable {
  public:
	Item(const std::string name, Rectangle hitbox, Rectangle renderSize, const sdl::Animation animation);
	Item(const Item &rhs);
	virtual ~Item();

	void render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
	            const sdl::RenderOptions &options = {})  override;

	const std::string name;
	Movable movable;
	// TODO behaviour

  private:
	Rectangle calcRenderTarget() const;

	const Rectangle renderSize;
	sdl::Animation animation;
};

class Door {
  public:
	Door(const std::string name, Item actualDoor, Direction direction, const std::string targetRoom, const std::string targetDoorName);
	Door(const Door &rhs);

	const std::string name;
	Item item;

	const Direction direction;
	const std::string targetRoom;
	const std::string targetDoorName;
};

} // namespace game

#endif /* ITEM_H */
