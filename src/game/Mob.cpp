#include "game/Mob.hpp"
game::Mob::Mob(const Mob &rhs)
    : name(rhs.name), walkingAnimation(rhs.walkingAnimation),
      idleAnimation(std::make_unique<sdl::Animation>(*rhs.idleAnimation))
{
}

game::Mob::Mob(const std::string name, const sdl::Animation walkingAnimation, OptionalAnimation idleAnimation)
    : name(name), walkingAnimation(walkingAnimation), idleAnimation(std::make_unique<sdl::Animation>(*idleAnimation))
{
}

game::Mob::~Mob() {}
