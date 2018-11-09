#ifndef MOB_H
#define MOB_H

#include "gamedef/EntityDefinitions.hpp"
#include "game/ResourceManager.hpp"

namespace game {

class Mob
{
public:
	Mob(const game_definitions::Mob &mobdef, ResourceManager &res);
	Mob(const Mob &rhs); // TODO set default state in copy ctor
	virtual ~Mob();

private:
};

}

#endif /* MOB_H */
