#ifndef FONT_H
#define FONT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <string>

#include "color.hpp"

namespace sdl {
struct Font {
	Font(TTF_Font *raw);

	TTF_Font *rawFont;

	Font() = delete;

	Font(const Font &) = delete;

	virtual ~Font();
};

// http://www.libsdl.org/projects/docs/SDL_ttf/SDL_ttf_35.html#SEC35
enum class TextRendering { Solid, Shaded, Blended };

} // namespace sdl

#endif /* FONT_H */
