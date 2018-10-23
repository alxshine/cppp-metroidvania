#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

namespace sdl {
struct Texture {
public:
  SDL_Texture *rawTexture;

  Texture(SDL_Texture *raw);

  Texture() = delete;

  Texture(const Texture &) = delete;

  Texture(const Texture &&) = delete;

  virtual ~Texture();
};
} // namespace sdl

#endif /* TEXTURE_H */
