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

#include "DefinitionLoading.hpp"

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
		throw ParseException("Excpected '" + wanted + "', got: " + got);
}

// Use AFTER reading keyword
std::istream &game_definitions::operator>>(std::istream &in, Rectangle &rect)
{
	in >> rect.topLeft;
	in >> rect.topRight;
	in >> rect.bottomLeft;
	in >> rect.bottomRight;

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
	testString("Debuff:", keyword);
	in >> attack.debuff;

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
	testString("Behaviour:", keyword);
	in >> mob.behaviour;

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
