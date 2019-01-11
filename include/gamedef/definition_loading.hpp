#ifndef DEFINITIONLOADING_H
#define DEFINITIONLOADING_H

#include <exception>
#include <istream>

#include "definition_loading.hpp"
#include "entity_definitions.hpp"

namespace game_definitions {

class ParseException : public std::exception {
	std::string s;

  public:
	ParseException(std::string s) : s(s) {}
	const char *what() const throw()
	{
		return s.c_str();
	}
};

std::istream &operator>>(std::istream &in, Rectangle &rect);

std::istream &operator>>(std::istream &in, Collision &coll);

std::istream &operator>>(std::istream &in, SoundEffect &snd);

std::istream &operator>>(std::istream &in, Animation &anim);

std::istream &operator>>(std::istream &in, Projectile &proj);

std::istream &operator>>(std::istream &in, Attack::Type &attack_type);

std::istream &operator>>(std::istream &in, Attack &attack);

std::istream &operator>>(std::istream &in, Mob &mob);

std::istream &operator>>(std::istream &in, Position &pos);

std::istream &operator>>(std::istream &in, Door &door);

std::istream &operator>>(std::istream &in, Item &item);

std::istream &operator>>(std::istream &in, MobRef &mobRef);

std::istream &operator>>(std::istream &in, ItemRef &itemRef);

std::istream &operator>>(std::istream &in, Room &room);

} // namespace game_definitions

#endif /* DEFINITIONLOADING_H */
