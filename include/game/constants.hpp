#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "gamedef/entity_definitions.hpp"

namespace game {

using Collision = game_definitions::Collision;
using CollisionMap = std::vector<std::vector<Collision>>;
using Direction = game_definitions::Direction;
using Position = game_definitions::Position;
using Rectangle = game_definitions::Rectangle;
using Health = game_definitions::Health;
using Line = sdl::Line;
using OptionalAnimation = std::unique_ptr<sdl::Animation>;

struct Attack {
	Attack(Rectangle hitBox, const sdl::Animation animation, int damage)
	    : hitBox(hitBox), animation(animation), damage(damage)
	{
	}
	Rectangle hitBox;
	sdl::Animation animation;
	int damage;
};

constexpr Rectangle tileSize{0, 0, 10, 10};
} // namespace game

#endif /* ifndef CONSTANTS_H */
