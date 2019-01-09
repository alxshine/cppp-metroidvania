#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "Color.hpp"
#include "GameClock.hpp"
#include "SdlException.hpp"
#include "geometry.hpp"

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
	Animation(const Texture &texture, std::vector<Rectangle> frames, GameClock::duration timePerFrame);
	const std::vector<Rectangle> &getFrames();

	inline int getFrameCount() const
	{
		return frames.size();
	}
	Sprite getAnimationFrame(GameClock::time_point t) const;
	Sprite getSprite(int index) const;
};

} // namespace sdl

#endif /* PRIMITIVES_H */
