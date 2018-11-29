#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <tuple>

#include <SDL2/SDL.h>

namespace sdl {
using Rectangle = SDL_Rect;
using Point = SDL_Point;
using Line = std::tuple<Point, Point>;

inline bool intersects(const Rectangle a, const Rectangle b)
{
	return SDL_HasIntersection(&a, &b) == SDL_TRUE ? true : false;
}

inline bool intersects(const Line a, const Rectangle b) {
	int x1 = std::get<0>(a).x;
	int y1 = std::get<0>(a).y;
	int x2 = std::get<1>(a).x;
	int y2 = std::get<1>(a).y;
	return SDL_IntersectRectAndLine(&b, &x1, &y1, &x2, &y2);
}

inline Rectangle calc_intersection(const Rectangle a, const Rectangle b)
{
	Rectangle result;
	SDL_IntersectRect(&a, &b, &result);
	return result;
}

inline bool contains(const Rectangle a, const Point b)
{
	return SDL_PointInRect(&b, &a) == SDL_TRUE ? true : false;
}

inline bool is_empty(const Rectangle a)
{
	return SDL_RectEmpty(&a) == SDL_TRUE ? true : false;
}

} // namespace sdl

// Need to be in global namespace for type aliases to work :/ - any solution?

inline bool operator==(const sdl::Rectangle a, const sdl::Rectangle b)
{
	return SDL_RectEquals(&a, &b) == SDL_TRUE ? true : false;
}

inline bool operator!=(const sdl::Rectangle a, const sdl::Rectangle b)
{
	return !(a == b);
}

inline bool operator==(const sdl::Point a, const sdl::Point b)
{
	return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const sdl::Point a, const sdl::Point b)
{
	return !(a == b);
}

inline sdl::Point operator*(const sdl::Point a, int b)
{
	return {a.x * b, a.y * b};
}

inline sdl::Point operator+(const sdl::Point a, const sdl::Point b)
{
	return {a.x + b.x, a.y + b.y};
}

inline sdl::Point &operator+=(sdl::Point &a, const sdl::Point b)
{
	a.x += b.x;
	a.y += b.y;
	return a;
}

#endif /* GEOMETRY_H */