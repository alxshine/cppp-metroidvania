#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "geometry.hpp"
#include "game/constants.hpp"

namespace game {
inline bool intersects(Rectangle lhs, Rectangle rhs)
{
	return sdl::intersects(lhs, rhs);
}
} // namespace game

#endif /* _PHYSICS_H_ */
