#include "game/movement.hpp"

game::Speed operator"" _ups(unsigned long long n)
{
	return game::Speed{(int)n};
}

game::Speed &game::Speed::operator+=(const game::Speed &rhs){
	s += rhs.s;
	return *this;
}

game::Speed operator+(game::Speed lhs, const game::Speed &rhs){
	lhs += rhs;
	return lhs;
}

game::Velocity &game::Velocity::operator+=(const game::Velocity &rhs){
	x += rhs.x;
	y += rhs.y;
	return *this;
}

game::Velocity operator+(game::Velocity lhs, const game::Velocity &rhs){
	lhs += rhs;
	return lhs;
}
