#include <texture.hpp>

#include "texture.hpp"

sdl::Texture::~Texture() {
	if (rawTexture != nullptr)
		SDL_DestroyTexture(rawTexture);
}

sdl::Texture::Texture(SDL_Texture *raw) {
	rawTexture = raw;
}
