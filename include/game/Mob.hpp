#ifndef MOB_H
#define MOB_H

#include "game/Movable.hpp"
#include "game/Attackable.hpp"
#include "SDL.hpp"
#include "constants.hpp"
#include "gamedef/entity_definitions.hpp"

namespace game {
class Mob : public sdl::Renderable {
  public:
	Mob(const std::string name, Health health, int speedPerSecond, Rectangle hitbox, Rectangle renderSize,
	    sdl::Animation walkingAnimation, OptionalAnimation idleAnimation, std::vector<Attack> attacks);
	Mob(const Mob &rhs);

	Mob &operator=(const Mob &rhs) = delete;
	virtual ~Mob();

	void render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
	            const sdl::RenderOptions &options = {}) override;
	Rectangle calcPositionedHitbox() const;

	const std::string name;
	Movable movable;
  Attackable attackable;

  private:
	const Rectangle hitbox;
	const Rectangle renderSize;
	sdl::Animation walkingAnimation;
	const OptionalAnimation idleAnimation;

	Rectangle calcRenderTarget() const;
};

} // namespace game

#endif /* MOB_H */
