#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include <string>

#include "color.hpp"

namespace sdl {
struct Texture {
	Texture(SDL_Texture *raw);

	SDL_Texture *rawTexture; //TODO: maybe we can do this better, but SDL_RenderCopy takes a non const SDL_Texture*

	void changeColor(Color color);

	Texture() = delete;

	Texture(const Texture &) = delete;

	Texture(const Texture &&) = delete;

	virtual ~Texture();
};

} // namespace sdl

#endif /* TEXTURE_H */
