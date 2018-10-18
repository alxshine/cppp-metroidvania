#ifndef ENTITY_DEFINITIONS_H
#define ENTITY_DEFINITIONS_H

#include <chrono>
#include <optional>
#include <ostream>
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

/*********************
 *  Print operators  *
 *********************/

inline std::ostream &operator<<(std::ostream &out, const Rectangle &rect)
{
	return out << "Rectangle {" << rect.topLeft << "," << rect.topRight << "," << rect.bottomLeft << ","
	           << rect.bottomRight << "}";
}

inline std::ostream &operator<<(std::ostream &out, const Animation &anim)
{
	out << "Animation { Spritesheet: " << anim.spritesheet << "; Time: " << anim.timePerFrame.count() << "; Frames: \n";
	for (auto f : anim.frames) {
		out << "\t" << f << "\n";
	}

	return out << "\t}\n";
}

inline std::ostream &operator<<(std::ostream &out, const Mob mob)
{
	out << "Mob " << mob.name << " {\n";
	out << "Health: " << mob.health << "\n";
	out << "WalkingAnimation: " << mob.walkingAnimation;

	if (mob.idleAnimation.spritesheet != "")
		out << "IdleAnimation: " << mob.idleAnimation;
	return out << "}\n";
}

} // namespace game_definitions

#endif // ENTITY_DEFINITIONS_H
