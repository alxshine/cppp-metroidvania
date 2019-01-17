#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <chrono>

#include <iostream>
#include <tuple>

#include "constants.hpp"

namespace game {
using Speed = int;
// struct Speed {
// Speed() : s{0} {}
// Speed(int n) : s{n} {}
// friend std::ostream &operator<<(std::ostream &out, const Speed &s);
// Speed &operator+=(const Speed &rhs);
// Speed &operator-=(const Speed &rhs);
// Speed &operator*=(const Speed &rhs);
// friend Speed operator+(Speed lhs, const Speed &rhs);
// friend Speed operator*(Speed lhs, const Speed &rhs);
// friend int operator*(const Speed &lhs, const std::chrono::milliseconds &rhs);
// friend bool operator<(const Speed &lhs, const Speed &rhs);
// friend bool operator>(const Speed &lhs, const Speed &rhs);

// private:
// int s;
//};

// Speed operator"" _ups(unsigned long long n);

struct PrecisePosition {
	float x, y;

	PrecisePosition() = default;
	PrecisePosition(float x, float y) : x(x), y(y) {}
	PrecisePosition(Position p) : x(static_cast<float>(p.x)), y(static_cast<float>(p.y)) {}
  
	inline int intX()
	{
		return static_cast<int>(x);
	}
	inline int intY()
	{
		return static_cast<int>(y);
	}

  inline Position toLowerPrecision() const{
    return {static_cast<int>(x), static_cast<int>(y)};
  }

	PrecisePosition &operator+=(const PrecisePosition &rhs);
  bool operator==(const PrecisePosition &rhs) const;
  bool operator!=(const PrecisePosition &rhs) const;
};

struct Velocity {
	Speed x, y;

	Velocity &operator+=(const Velocity &rhs);
	friend Velocity operator+(Velocity lhs, const Velocity &rhs);
	friend PrecisePosition operator*(const Velocity &lhs, std::chrono::milliseconds rhs);
};

} // namespace game
#endif /* ifndef MOVEMENT_H */
