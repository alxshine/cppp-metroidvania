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

inline std::ostream &operator<<(std::ostream &out, const Rectangle &r)
{
	return out << "{" << r.x << ", " << r.y << ", " << r.w << ", " << r.h << "}";
}

inline std::ostream &operator<<(std::ostream &out, const Position &p)
{
	return out << "{" << p.x << ", " << p.y << "}";
}

constexpr Rectangle tileSize{0, 0, 10, 10};

  
inline Rectangle tileRectangle(int row, int column)
{
	return {column * tileSize.w, row * tileSize.h, tileSize.w, tileSize.h};
}
} // namespace game

#endif /* ifndef CONSTANTS_H */
