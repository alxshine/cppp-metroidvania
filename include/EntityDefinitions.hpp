#ifndef ENTITY_DEFINITIONS_H
#define ENTITY_DEFINITIONS_H

#include <chrono>
#include <optional>
#include <vector>

/**********************************************
*  Classes to parse our config into memory.  *
**********************************************/


namespace game_definitions {

using Health = int;

struct Rectangle {
	int topLeft;
	int topRight;
	int bottomLeft;
	int bottomRight;
};

struct Animation {
	std::string spritesheet;
	std::chrono::milliseconds timePerFrame;
	std::vector<Rectangle> frames;
};

struct Attack {
};

struct Mob {
  public:
	std::string name;
	Health health;
	Animation walkingAnimation;
	Animation idleAnimation;
	std::vector<Attack> attacks;
};

} // namespace game_definitions

#endif // ENTITY_DEFINITIONS_H
