#ifndef DEFINITIONLOADING_H
#define DEFINITIONLOADING_H

#include <exception>
#include <istream>

#include "EntityDefinitions.hpp"

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

std::istream &operator>>(std::istream &in, Animation &anim);

std::istream &operator>>(std::istream &in, Mob &mob);

} // namespace gamedef

#endif /* DEFINITIONLOADING_H */
