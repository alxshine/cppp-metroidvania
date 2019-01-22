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
	Attack(Rectangle hitBox, sdl::Animation animation, int damage, std::vector<int> damageFrames)
	    : hitBox(hitBox), animation(std::move(animation)), damage(damage), damageFrames(std::move(damageFrames))
	{
	}
	Rectangle hitBox;
	sdl::Animation animation;
	int damage;
	std::vector<int> damageFrames;
};

inline std::ostream &operator<<(std::ostream &out, const Rectangle &r)
{
	return out << "{" << r.x << ", " << r.y << ", " << r.w << ", " << r.h << "}";
}

inline std::ostream &operator<<(std::ostream &out, const Position &p)
{
	return out << "{" << p.x << ", " << p.y << "}";
}

constexpr Rectangle tileSize{0, 0, 10, 10};
} // namespace game

#endif /* ifndef CONSTANTS_H */
