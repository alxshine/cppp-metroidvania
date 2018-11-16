#ifndef ROOM_H
#define ROOM_H

#include "gamedef/entity_definitions.hpp"

namespace game {

struct Room {
  public:
	Room();                // TODO ctor
	Room(const Room &rhs); // TODO set default state in copy ctor
	Room &operator=(const Room &rhs) = delete;
	virtual ~Room();

  private:
};

} // namespace game

#endif /* ROOM_H */
