#include "Font.hpp"
#include "SdlException.hpp"

sdl::Font::~Font()
{
	if (rawFont != nullptr)
		TTF_CloseFont(rawFont);
}

sdl::Font::Font(TTF_Font *raw) : rawFont(raw) {}

sdl::Font::Font(Font &&rhs) : rawFont(rhs.rawFont)
{
	rhs.rawFont = nullptr;
};
