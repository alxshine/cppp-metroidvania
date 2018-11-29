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
	// TODO keep track of whether we moved or not in order to choose idle/walking animation
	// TODO use position for rendering target rectangle
	renderer.render(walkingAnimation.getAnimationFrame(t));
}
