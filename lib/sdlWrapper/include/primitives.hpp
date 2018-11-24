#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include <string>
#include <vector>

#include "Color.hpp"
#include "GameClock.hpp"
#include "Rectangle.hpp"
#include "SdlException.hpp"

namespace sdl {
struct Texture final {
	Texture(SDL_Texture *raw);

	SDL_Texture *rawTexture;

	Texture() = delete;

	Texture(const Texture &) = delete;

	Texture(Texture &&rhs);

	~Texture();
};

struct Sprite final {
  public:
	const Texture &texture;
	Rectangle sourceRectangle;
};

using Text = Sprite;

struct Animation {
  private:
	const Texture &texture;
	std::vector<Rectangle> frames;
	GameClock::duration timePerFrame;

  public:
	Animation(const Texture& texture, std::vector<Rectangle> frames, GameClock::duration timePerFrame);

	Sprite getAnimationFrame(GameClock::time_point t) const;
};

} // namespace sdl

#endif /* PRIMITIVES_H */
