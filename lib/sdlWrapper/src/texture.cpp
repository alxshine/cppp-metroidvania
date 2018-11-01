#include "texture.hpp"
#include "sdlException.hpp"

sdl::Texture::~Texture()
{
	if (rawTexture != nullptr)
		SDL_DestroyTexture(rawTexture);
}

sdl::Texture::Texture(SDL_Texture *raw) : rawTexture(raw) {}

void sdl::Texture::changeColor(Color color)
{
	if (SDL_SetTextureColorMod(rawTexture, color.r, color.g, color.b))
		throw SdlException(SDL_GetError());
}

sdl::Texture::Texture(Texture &&rhs) : rawTexture(rhs.rawTexture)
{
	rhs.rawTexture = nullptr;
}
