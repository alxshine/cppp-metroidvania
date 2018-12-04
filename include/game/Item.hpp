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
	Item &operator=(const Item &rhs) = delete;
	virtual ~Item();

	void render(const sdl::Renderer &renderer, const sdl::GameClock::time_point &t,
	            const sdl::RenderOptions &options = {}) const override;
	Rectangle calcPositionedHitbox() const;

	const std::string name;
	Movable movable;
	// TODO behaviour

  private:
	Rectangle calcRenderTarget() const;

	const Rectangle hitbox;
	const Rectangle renderSize;
	const sdl::Animation animation;
};

class Door {
  public:
	Item item;

	const Direction direction;
	const std::string name;
	const std::string targetRoom;
	const std::string targetDoorName;
};

} // namespace game

#endif /* ITEM_H */
