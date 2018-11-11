#ifndef MOB_H
#define MOB_H

#include "gamedef/EntityDefinitions.hpp"
#include "SDL.hpp"

namespace game {

using OptionalAnimation = std::unique_ptr<sdl::Animation>;

struct Mob {
  public:
	Mob(const std::string name, const sdl::Animation walkingAnimation, OptionalAnimation idleAnimation); // TODO finish ctor
	Mob(const Mob &rhs); // TODO set default state in copy ctor

	Mob &operator=(const Mob &rhs) = delete;
	virtual ~Mob();

	const std::string name;
	const sdl::Animation walkingAnimation;
	const OptionalAnimation idleAnimation;
};

} // namespace game

#endif /* MOB_H */
