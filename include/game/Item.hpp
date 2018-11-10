#ifndef ITEM_H
#define ITEM_H

#include "game/ResourceManager.hpp"
#include "gamedef/EntityDefinitions.hpp"

namespace game {

class Item {
  public:
	Item();                // TODO ctor
	Item(const Item &rhs); // TODO set default state in copy ctor
	Item &operator=(const Item &rhs) = delete;
	virtual ~Item();

  private:
	// TODO members
};

} // namespace game

#endif /* ITEM_H */
