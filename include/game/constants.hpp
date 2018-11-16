#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "gamedef/entity_definitions.hpp"

namespace game {
using Rectangle = game_definitions::Rectangle;
using Position = game_definitions::Position;
constexpr Rectangle tileSize{0, 0, 10, 10};
} // namespace game

#endif /* ifndef CONSTANTS_H */
