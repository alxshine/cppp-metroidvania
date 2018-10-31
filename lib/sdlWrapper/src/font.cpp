#include "font.hpp"
#include "sdlException.hpp"

sdl::Font::~Font()
{
	if (rawFont != nullptr)
		TTF_CloseFont(rawFont);
}

sdl::Font::Font(TTF_Font *raw) : rawFont(raw) {}
