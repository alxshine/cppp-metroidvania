#ifndef MOB_H
#define MOB_H

#include "gamedef/entity_definitions.hpp"
#include "SDL.hpp"

namespace game {

using OptionalAnimation = std::unique_ptr<sdl::Animation>;

class Mob : public sdl::Renderable {
  public:
	Mob(const std::string name, const sdl::Animation walkingAnimation, OptionalAnimation idleAnimation); // TODO finish ctor
	Mob(const Mob &rhs); // TODO set default state in copy ctor

	Mob &operator=(const Mob &rhs) = delete;
	virtual ~Mob();

	void render(const sdl::Renderer &renderer, const sdl::GameClock::time_point &t, const sdl::RenderOptions &options = {}) const override;

	const std::string name;
	const sdl::Animation walkingAnimation;
	const OptionalAnimation idleAnimation;
};

} // namespace game

#endif /* MOB_H */
