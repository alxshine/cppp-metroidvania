#include "game/Mob.hpp"
// TODO: if we get problems with the vector automatically calling this and resetting values, maybe add a copy ctor with
// position since we will need to position mobs on spawning anyway
game::Mob::Mob(const Mob &rhs)
    : name(rhs.name), movable(rhs.movable), walkingAnimation(rhs.walkingAnimation),
      idleAnimation(std::make_unique<sdl::Animation>(*rhs.idleAnimation))
{
}

game::Mob::Mob(const std::string name, int speedPerSecond, const sdl::Animation walkingAnimation,
               OptionalAnimation idleAnimation)
    : name(name), movable(speedPerSecond), walkingAnimation(walkingAnimation), idleAnimation(std::move(idleAnimation))
{
}

game::Mob::~Mob() {}

void game::Mob::render(const sdl::Renderer &renderer, const sdl::GameClock::time_point &t,
                       const sdl::RenderOptions &options) const
{
	// Calculate position, centering horizontally and bottom-aligning vertically
	// TODO: specify number of tiles to use in definition instead of fixed 2x2
	Rectangle destRect{movable.getPosition().x - tileSize.w, movable.getPosition().y - 2 * tileSize.h, tileSize.w * 2,
	                   tileSize.h * 2};

	sdl::Renderer::Flip flip;
	switch (movable.getDirection()) {
	case Direction::Up:
		// fallthrough
	case Direction::Down:
		// fallthrough
	case Direction::Right:
		flip = sdl::Renderer::Flip::None;
		break;
	case Direction::Left:
		flip = sdl::Renderer::Flip::X;
		break;
	}

	if (movable.getMoved() || idleAnimation == nullptr)
		renderer.render(walkingAnimation.getAnimationFrame(t), destRect, flip);
	else
		renderer.render(idleAnimation->getAnimationFrame(t), destRect, flip);

	// draw texture box
	if (options.renderEntityDrawRectangles)
		renderer.drawRectangle(destRect, {0, 0, 255, 128}, false);

	// TODO draw hit/collision box once we store it in mob
}
