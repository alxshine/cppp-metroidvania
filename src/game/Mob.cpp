#include "game/Mob.hpp"
game::Mob::Mob(const Mob &rhs)
    : name(rhs.name), walkingAnimation(rhs.walkingAnimation),
      idleAnimation(std::make_unique<sdl::Animation>(*rhs.idleAnimation))
{
}

game::Mob::Mob(const std::string name, const sdl::Animation walkingAnimation, OptionalAnimation idleAnimation)
    : name(name), walkingAnimation(walkingAnimation), idleAnimation(std::move(idleAnimation))
{
}

game::Mob::~Mob() {}

void game::Mob::render(const sdl::Renderer &renderer, const sdl::GameClock::time_point &t) const
{
	// TODO keep track of whether we moved or not in order to choose idle/walking animation
	// TODO use position for rendering target rectangle
	renderer.render(walkingAnimation.getAnimationFrame(t));
}

