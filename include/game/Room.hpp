#ifndef ROOM_H
#define ROOM_H

#include "gamedef/EntityDefinitions.hpp"
#include "game/ResourceManager.hpp"

namespace game {

class Room
{
public:
	Room(const game_definitions::Room &roomdef, ResourceManager &res);
	Room(const Room &rhs); // TODO set default state in copy ctor
	virtual ~Room();

private:
};

}

#endif /* ROOM_H */
