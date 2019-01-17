#include "game/movement.hpp"

namespace game {

// game::Speed operator"" _ups(unsigned long long n)
//{
// return game::Speed{(int)n};
//}

// game::Speed &game::Speed::operator+=(const game::Speed &rhs)
//{
// s += rhs.s;
// return *this;
//}

// game::Speed &game::Speed::operator-=(const game::Speed &rhs)
//{
// s -= rhs.s;
// return *this;
//}

// game::Speed &game::Speed::operator*=(const game::Speed &rhs)
//{
// s *= rhs.s;
// return *this;
//}

// game::Speed operator+(game::Speed lhs, const game::Speed &rhs)
//{
// lhs += rhs;
// return lhs;
//}

// game::Speed operator-(game::Speed lhs, const game::Speed &rhs)
//{
// lhs -= rhs;
// return lhs;
//}

// game::Speed operator*(game::Speed lhs, const game::Speed &rhs)
//{
// lhs *= rhs;
// return lhs;
//}

// int operator*(const game::Speed &lhs, const std::chrono::milliseconds &rhs)
//{
// return lhs.s * rhs.count() / 1000;
//}

// bool operator<(const game::Speed &lhs, const game::Speed &rhs)
//{
// return lhs.s < rhs.s;
//}

// bool operator>(const game::Speed &lhs, const game::Speed &rhs)
//{
// return lhs.s > rhs.s;
//}

game::PrecisePosition &game::PrecisePosition::operator+=(const PrecisePosition &rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
};

bool game::PrecisePosition::operator==(const PrecisePosition &rhs) const
{
	return x == rhs.x && y == rhs.y;
}

bool game::PrecisePosition::operator!=(const PrecisePosition &rhs) const
{
	return x != rhs.x || y != rhs.y;
}
game::Velocity &game::Velocity::operator+=(const game::Velocity &rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

game::Velocity operator+(game::Velocity lhs, const game::Velocity &rhs)
{
	lhs += rhs;
	return lhs;
}

game::PrecisePosition operator*(const game::Velocity &lhs, std::chrono::milliseconds rhs)
{
	float x = static_cast<float>(lhs.x) * rhs.count() / 1000;
	float y = static_cast<float>(lhs.y) * rhs.count() / 1000;
	return {x, y};
}

} // namespace game
