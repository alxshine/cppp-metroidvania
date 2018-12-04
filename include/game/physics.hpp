#ifndef PHYSICS_H
#define PHYSICS_H

#include "Game.hpp"

namespace game {

bool collidesLeft(Rectangle playerHitBox, Room &currentRoom);

bool collidesRight(Rectangle playerHitBox, Room &currentRoom);

bool collidesTop(Rectangle playerHitBox, Room &currentRoom);

bool collidesBottom(Rectangle playerHitBox, Room &currentRoom);

bool isStanding(Rectangle playerHitBox, Room &currentRoom);

void resolvePlayerCollision(Player &player, Room &currentRoom);

} // namespace game

#endif /* PHYSICS_H */
