#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <chrono>

#include <iostream>
#include <tuple>

#include "constants.hpp"

namespace game {
struct Speed {
	Speed() : s{0} {}
	Speed(int n) : s{n} {}
	friend std::ostream &operator<<(std::ostream &out, const Speed &s);
	operator int() const noexcept
	{
		return s;
	}
	Speed &operator+=(const Speed &rhs);
	Speed &operator-=(const Speed &rhs);
	friend Speed operator+(Speed lhs, const Speed &rhs);
	friend bool operator<(const Speed &lhs, const Speed &rhs);
	friend bool operator>(const Speed &lhs, const Speed &rhs);

  private:
	int s;
};

Speed operator"" _ups(unsigned long long n);

struct Velocity {
	Speed x, y;

	Velocity &operator+=(const Velocity &rhs);
	friend Velocity operator+(Velocity lhs, const Velocity &rhs);
	friend Position operator*(const Velocity &lhs, std::chrono::milliseconds rhs);
};

} // namespace game
#endif /* ifndef MOVEMENT_H */
