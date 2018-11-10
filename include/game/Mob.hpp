#ifndef MOB_H
#define MOB_H

#include "game/ResourceManager.hpp"
#include "gamedef/EntityDefinitions.hpp"

namespace game {

class Mob {
  public:
	Mob();               // TODO ctor
	Mob(const Mob &rhs); // TODO set default state in copy ctor
	Mob &operator=(const Mob &rhs) = delete;
	virtual ~Mob();

  private:
	// TODO members
};

} // namespace game

#endif /* MOB_H */
