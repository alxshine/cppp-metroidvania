/**
 * @file DefinitionLoading.cpp
 * @brief Helpers to load game definitions.
 * @author bennett
 * @version 0
 * @date 2018-10-18
 *
 * Principle:
 * - detect keyword
 * - load corresponding entity using operator>>
 *
 * If necessary, the entities read until their end token.
 */

#include <regex>
#include <string>

#include "gamedef/definition_loading.hpp"

using namespace game_definitions;
using namespace std::string_literals;

/**
 * @brief Throw a ParseException if the given strings don't match.
 *
 * @param wanted Expected string
 * @param got Actual string.
 */
void testString(const std::string &wanted, const std::string &got)
{
	if (wanted != got)
		throw ParseException("Expected '" + wanted + "', got: " + got);
}

// Use AFTER reading keyword
std::istream &game_definitions::operator>>(std::istream &in, Rectangle &rect)
{
	in >> rect.x;
	in >> rect.y;
	in >> rect.w;
	in >> rect.h;

	return in;
}

std::istream &game_definitions::operator>>(std::istream &in, Collision &coll){
	std::string rawValue;
	in >> rawValue;
	if(rawValue == "None"){
		coll = Collision::None;
	}else if(rawValue == "TopOnly"){
		coll = Collision::TopOnly;
	}else if(rawValue == "Full"){
		coll = Collision::Full;
	}else{
		throw "Invalid Collision type " + rawValue;
	}
	return in;
}

std::istream &game_definitions::operator>>(std::istream &in, SoundEffect &snd)
{
	return in >> snd.file >> snd.loops;
}

std::istream &game_definitions::operator>>(std::istream &in, Animation &anim)
{
	std::string keyword;

	in >> keyword;
	testString("Spritesheet:", keyword);
	in >> anim.spritesheet;

	in >> keyword;
	testString("Time:", keyword);
	int ms;
	in >> ms;
	anim.timePerFrame = std::chrono::milliseconds(ms);

	// Parse all frames until EndAnimation
	while (true) {
		in >> keyword;
		if (keyword == "Frame:") {
			Rectangle rect;
			in >> rect;
			anim.frames.push_back(rect);
		} else if (keyword == "EndAnimation")
			break;
		else
			throw ParseException("Unterminated Animation");
	}

	return in;
}

std::istream &game_definitions::operator>>(std::istream &in, Projectile &proj)
{
	std::string keyword;

	in >> keyword;
	testString("TravelSpeed:", keyword);
	in >> proj.travelSpeed;

	in >> keyword;
	testString("ImpactSound:", keyword);
	in >> proj.impactSound;

	in >> keyword;
	testString("NoClip:", keyword);
	std::string noclipValue;
	in >> noclipValue;
	proj.noclip = noclipValue == "True" ? true : false;

	in >> keyword;
	testString("TravelAnimation:", keyword);
	in >> proj.travelAnimation;

	in >> keyword;
	testString("ImpactAnimation:", keyword);
	in >> proj.impactAnimation;

	return in;
}

std::istream &game_definitions::operator>>(std::istream &in, Attack::Type &type)
{
	std::string value;
	in >> value;
	if (value == "Melee")
		type = Attack::Type::Melee;
	else if (value == "Ranged")
		type = Attack::Type::Ranged;
	else
		throw ParseException("Invalid Attack Type: " + value);

	return in;
}

std::istream &game_definitions::operator>>(std::istream &in, Attack &attack)
{
	std::string keyword;

	in >> attack.name;

	in >> keyword;
	testString("Type:", keyword);
	in >> attack.type;

	in >> keyword;
	testString("SoundEffect:", keyword);
	in >> attack.soundEffect;

	in >> keyword;
	testString("HitBox:", keyword);
	in >> attack.hitbox;

	in >> keyword;
	testString("Damage:", keyword);
	in >> attack.damage;

	in >> keyword;
	testString("Animation:", keyword);
	in >> attack.animation;

	// Melee attacks are finished here
	if (attack.type == Attack::Type::Melee)
		return in;

	in >> keyword;
	testString("Projectile:", keyword);
	in >> attack.projectile;

	return in;
}

std::istream &game_definitions::operator>>(std::istream &in, Mob &mob)
{
	std::string keyword;

	in >> keyword;
	testString("Name:", keyword);
	in >> mob.name;

	in >> keyword;
	testString("Health:", keyword);
	in >> mob.health;

	in >> keyword;
	testString("Speed:", keyword);
	in >> mob.speedPerSecond;

	in >> keyword;
	testString("Behaviour:", keyword);
	in >> mob.behaviour;

	in >> keyword;
	testString("Hitbox:", keyword);
	in >> mob.hitbox;

	in >> keyword;
	testString("DrawSize:", keyword);
	in >> mob.drawSize;

	in >> keyword;
	testString("WalkingAnimation:", keyword);
	in >> mob.walkingAnimation;

	in >> keyword;
	if (keyword == "IdleAnimation:")
		in >> mob.idleAnimation;
	else if (keyword != "NoIdleAnimation")
		throw new ParseException("Idle Animation not defined or declared empty");

	// Parse all attacks until EndMob
	while (true) {
		keyword = "";
		in >> keyword;
		if (keyword == "Attack:") {
			Attack attack;
			in >> attack;
			mob.attacks.push_back(attack);
		} else if (keyword == "EndMob")
			break;
		else
			throw ParseException("Unterminated Mob");
	}

	return in;
}

std::istream &game_definitions::operator>>(std::istream &in, Door &door)
{
	in >> door.position >> door.name;
	in.ignore(2);
	in >> door.itemId;

	std::string dir;
	in >> dir;
	if (dir == "(Left)")
		door.direction = {-1,0};
	else if (dir == "(Down)")
		door.direction = {0,1};
	else if (dir == "(Right)")
		door.direction = {1,0};
	else if (dir == "(Up)")
		door.direction = {0,-1};
	else
		throw ParseException("Unkown direction " + dir);

	in.ignore(3);
	in >> door.targetDoorName;
	in.ignore(3);
	in >> door.targetRoom;

	return in;
}

std::istream &game_definitions::operator>>(std::istream &in, Item &item)
{
	std::string keyword;

	in >> keyword;
	testString("Name:", keyword);
	in >> item.name;

	in >> keyword;
	testString("Behaviour:", keyword);
	in >> item.behaviour;

	in >> keyword;
	testString("Hitbox:", keyword);
	in >> item.hitbox;

	in >> keyword;
	testString("DrawSize:", keyword);
	in >> item.drawSize;

	in >> keyword;
	testString("Animation:", keyword);
	in >> item.animation;

	return in;
}

std::istream &game_definitions::operator>>(std::istream &in, MobRef &mobRef)
{
	return in >> mobRef.position >> mobRef.id;
}

std::istream &game_definitions::operator>>(std::istream &in, ItemRef &itemRef)
{
	return in >> itemRef.position >> itemRef.id;
}

std::istream &game_definitions::operator>>(std::istream &in, Position &pos)
{
	return in >> pos.x >> pos.y;
}

std::istream &game_definitions::operator>>(std::istream &in, Room &room)
{
	std::string keyword;

	in >> keyword;
	testString("Name:", keyword);
	in >> room.name;

	in >> keyword;
	testString("Background:", keyword);
	in >> room.background;

	in >> keyword;
	testString("Music:", keyword);
	in >> room.music;

	in >> keyword;
	testString("Location:", keyword);
	in >> room.location;

	in >> keyword;
	testString("Tileset:", keyword);
	in >> room.tileset;

	// Parse Tiles
	in >> keyword;
	testString("Tiles:", keyword);
	std::map<char, Tile> tileMap;

	while (true) {
		keyword = "";
		in >> keyword;
		if (keyword == "EndTiles")
			break;

		Tile tile;
		in >> tile.name;
		in >> tile.rectangle;
		in >> tile.collision;
		tileMap.insert({keyword[0], tile});
	}

	// Parse layout
	in >> keyword;
	testString("Layout:", keyword);
	while (true) {
		keyword = "";
		in >> keyword;
		if (keyword == "EndLayout")
			break;

		testString("Layer:", keyword);
		std::vector<TileRow> layer;
		while (true) {
			keyword = "";
			in >> keyword;
			if (keyword == "EndLayer")
				break;

			// Process line of room
			TileRow line;
			for (char t : keyword) {
				// Add the correct tiles, *hoping that they exist in the map :)*
				line.push_back(tileMap.find(t)->second);
			}
			layer.push_back(line);
		}
		room.layout.push_back(layer);
	}

	// Parse entities
	in >> keyword;
	testString("Entities:", keyword);
	while (true) {
		keyword = "";
		in >> keyword;
		if (keyword == "Mob:") {
			MobRef mobRef;
			in >> mobRef;
			room.mobs.push_back(mobRef);
		} else if (keyword == "Item:") {
			ItemRef itemRef;
			in >> itemRef;
			room.items.push_back(itemRef);
		} else if (keyword == "Door:") {
			Door door;
			in >> door;
			room.doors.push_back(door);
		} else if (keyword == "EndEntities")
			break;
		else
			throw ParseException("Unterminated Room");
	}

	return in;
}
