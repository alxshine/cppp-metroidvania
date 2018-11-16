#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <SDL2/SDL.h>

namespace sdl {
using Rectangle = SDL_Rect;

inline bool intersects(const Rectangle &a, const Rectangle &b)
{
	return SDL_HasIntersection(&a, &b) == SDL_TRUE ? true : false;
}
} // namespace sdl

#endif /* RECTANGLE_H */
