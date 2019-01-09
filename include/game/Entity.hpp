#ifndef ENTITY_H
#define ENTITY_H

#include "Movable.hpp"
#include "SDL.hpp"
#include "constants.hpp"

namespace game {
class Entity : public sdl::Renderable {
  public:
	Entity(const sdl::Animation idleAnimation, const sdl::Animation walkingAnimation);

	Entity &operator=(const Entity &rhs) = delete;
	Entity(const Entity &rhs) = delete;

	void render(const sdl::Renderer &renderer, const sdl::GameClock::time_point &t,
	            const sdl::RenderOptions &options = {}) const override;

	static constexpr Rectangle hitbox{static_cast<int>(tileSize.w / 2), 0, tileSize.w, tileSize.h * 2};
	Movable movable;

	Rectangle calcPositionedHitbox() const;
	Rectangle calcLastPositionedHitbox() const;

  private:
	Rectangle calcRenderTarget() const;
	const sdl::Animation idleAnimation;
};
} // namespace game

#endif /* ENTITY_H */
