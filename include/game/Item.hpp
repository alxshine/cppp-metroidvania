#ifndef ITEM_H
#define ITEM_H

#include "gamedef/EntityDefinitions.hpp"
#include "game/ResourceManager.hpp"

namespace game {

class Item
{
public:
	Item(const game_definitions::Item &itemdef, ResourceManager &res);
	Item(const Item &rhs); // TODO set default state in copy ctor
	virtual ~Item();

private:
};

}

#endif /* ITEM_H */
