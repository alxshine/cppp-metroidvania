#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "gamedef/entity_definitions.hpp"

namespace game {

using Collision = game_definitions::Collision;
using Direction = game_definitions::Direction;
using Position = game_definitions::Position;
using Rectangle = game_definitions::Rectangle;

constexpr Rectangle tileSize{0, 0, 10, 10};
} // namespace game

#endif /* ifndef CONSTANTS_H */
