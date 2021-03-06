#ifndef ENTITY_DEFINITIONS_H
#define ENTITY_DEFINITIONS_H

#include <chrono>
#include <optional>
#include <ostream>
#include <vector>

#include "SDL.hpp"

/**********************************************
 *  Classes to parse our config into memory.  *
 **********************************************/

namespace game_definitions {

using Health = int;
using Damage = int;

using Rectangle = sdl::Rectangle;
using Position = sdl::Point;

struct SoundEffect {
	std::string file;
	int loops;
};

struct Animation {
	std::string spritesheet;
	std::chrono::milliseconds timePerFrame;
	std::vector<Rectangle> frames;
};

struct Projectile {
	Rectangle hitBox;
	int damage;
	bool noClip;
	int maxSpeed;
	Position startPosition;

	Animation animation;
};

struct Attack {
	std::string name;
	enum class Type { Melee, Ranged } type;
	SoundEffect soundEffect;
	Rectangle hitbox;
	Damage damage;
	std::vector<int> damageFrames;

	Animation animation;
	Projectile projectile;
};

struct Mob {
	std::string name;
	std::string behaviour;
	Health health;
	int poise;
	int recoveryWindow;
	Rectangle hitbox;
	Rectangle drawSize;
	int speedPerSecond;

	Animation walkingAnimation;
	Animation deathAnimation;
	Animation hurtAnimation;
	Animation idleAnimation;

	std::vector<Attack> attacks;
};

struct Direction {
	int x, y;
};

struct Door {
	Position position;
	Direction direction;
	std::string name;
	std::string itemId;
	std::string targetRoom;
	std::string targetDoorName;
};

struct MobRef {
	Position position;
	std::string id;
};

struct Item {
	std::string name;
	std::string description;
	Rectangle hitbox;
	Rectangle drawSize;
	Animation animation;
	std::string behaviour;
};

struct ItemRef {
	Position position;
	std::string id;
};

enum class Collision : char { None = 0, TopOnly, Full };

struct Tile {
	std::string name;
	Rectangle rectangle;
	Collision collision;
};

using TileRow = std::vector<Tile>;
using TileLayer = std::vector<TileRow>;

struct Room {
	std::string name;
	std::string background;
	std::string music;
	Position location;
	std::string tileset;
	std::vector<TileLayer> layout;
	int gatingArea;

	std::vector<MobRef> mobs;
	std::vector<Door> doors;
	std::vector<Door> onClearDoors;
	std::vector<ItemRef> items;
	std::vector<ItemRef> onClearItems;
};

/*********************
 *  Print operators  *
 *********************/
inline std::ostream &operator<<(std::ostream &out, const SDL_Point &pos)
{
	return out << pos.x << " " << pos.y << "\n";
}

inline std::ostream &operator<<(std::ostream &out, const Rectangle &rect)
{
	return out << "Rectangle {" << rect.x << "," << rect.y << "," << rect.w << "," << rect.h << "}\n";
}

inline std::ostream &operator<<(std::ostream &out, const SoundEffect &snd)
{
	return out << "SoundEffect { File: " << snd.file << "; Loops: " << snd.loops << ";}\n";
}

inline std::ostream &operator<<(std::ostream &out, const Animation &anim)
{
	out << "Animation { Spritesheet: " << anim.spritesheet << "; Time: " << anim.timePerFrame.count() << "; Frames: \n";
	for (auto f : anim.frames) {
		out << "\t" << f;
	}

	return out << "\t}\n";
}

inline std::ostream &operator<<(std::ostream &out, const Projectile &proj)
{
	out << "Projectile { Hitbox: " << proj.hitBox << "\n";
	out << "Damage: " << proj.damage << "\n";
	out << "NoClip: " << proj.noClip << "\n";
	out << "MaxSpeed: " << proj.maxSpeed << "\n";
	out << "StartPosition: " << proj.startPosition << "\n";
	out << "TravelAnimation: " << proj.animation;

	return out << "}\n";
}

inline std::ostream &operator<<(std::ostream &out, const Attack::Type &type)
{
	switch (type) {
	case Attack::Type::Melee:
		return out << "Melee";
	case Attack::Type::Ranged:
		return out << "Ranged";
	}
	return out;
}

inline std::ostream &operator<<(std::ostream &out, const Attack &attack)
{
	out << "Attack { Name: " << attack.name << "; Type: " << attack.type << ";\n";
	out << "SoundEffect: " << attack.soundEffect;
	out << "HitBox: " << attack.hitbox;
	out << "Damage: " << attack.damage << "\n";

	out << "Animation: " << attack.animation;

	if (attack.type == Attack::Type::Ranged)
		out << "Projectile: " << attack.projectile;

	return out << "}\n";
}

inline std::ostream &operator<<(std::ostream &out, const Mob &mob)
{
	out << "Mob " << mob.name << " {\n";
	out << "Health: " << mob.health << "\n";
	out << "Behaviour: " << mob.behaviour << "\n";
	out << "WalkingAnimation: " << mob.walkingAnimation;

	if (mob.idleAnimation.spritesheet != "")
		out << "IdleAnimation: " << mob.idleAnimation;

	out << "Attacks: \n";
	for (auto a : mob.attacks) {
		out << a;
	}

	return out << "}\n";
}

} // namespace game_definitions

#endif // ENTITY_DEFINITIONS_H
