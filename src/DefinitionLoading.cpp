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

// TODO should these be declared in the header?

// Use AFTER reading keyword
std::istream &game_definitions::operator>>(std::istream &in, Rectangle &rect)
{
	in >> rect.topLeft;
	in >> rect.topRight;
	in >> rect.bottomLeft;
	in >> rect.bottomRight;

	return in;
}

// Use AFTER reading keyword
std::istream &game_definitions::operator>>(std::istream &in, Animation &anim)
{
	std::string spritesheet;
	in >> spritesheet;
	testString("Spritesheet:", spritesheet);
	in >> anim.spritesheet;

	std::string time;
	in >> time;
	testString("Time:", time);
	int ms;
	in >> ms;
	anim.timePerFrame = std::chrono::milliseconds(ms);

	// Parse all frames until EndAnimation
	std::string keyword;
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

std::istream &game_definitions::operator>>(std::istream &in, Mob &mob)
{
	std::string name;
	in >> name;
	testString("Name:", name);
	in >> mob.name;

	std::string health;
	in >> health;
	testString("Health:", health);
	in >> mob.health;

	std::string walkingAnimation;
	in >> walkingAnimation;
	testString("WalkingAnimation:", walkingAnimation);
	in >> mob.walkingAnimation;

	std::string token;
	in >> token;
	if (token == "IdleAnimation:")
		in >> mob.idleAnimation;

	// TODO handle attacks
	return in;
}
