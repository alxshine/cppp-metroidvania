#ifndef MOB_H
#define MOB_H

#include "SDL.hpp"
#include "constants.hpp"
#include "game/AI.hpp"
#include "game/Attackable.hpp"
#include "game/Movable.hpp"
#include "gamedef/entity_definitions.hpp"

namespace game {
class Mob : public sdl::Renderable {
  public:
	Mob(const std::string name, Health health, int poise, sdl::GameClock::duration invulnerabilityWindow, int speedPerSecond, Rectangle hitbox, Rectangle renderSize,
	    sdl::Animation walkingAnimation, sdl::Animation deathAnimation, sdl::Animation hurtAnimation, sdl::Animation idleAnimation,
	    std::vector<Attack> attacks, std::shared_ptr<AI> ai);
	Mob(const Mob &rhs);

	virtual ~Mob();

	void render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
	            const sdl::RenderOptions &options = {}) override;
	void performAiStep(const CollisionMap &collisionMap, Rectangle playerHitBox);
	bool isNeededOnScreen();

	std::string name;
	Movable movable;
	Attackable attackable;

	sdl::Animation idleAnimation;

  private:
	Rectangle renderSize;
	sdl::Animation walkingAnimation;
	std::shared_ptr<AI> ai;

	Rectangle calcRenderTarget() const;
};

inline bool operator<(const Mob &a, const Mob&b)
{
	return a.name < b.name;
}

} // namespace game

#endif /* MOB_H */
