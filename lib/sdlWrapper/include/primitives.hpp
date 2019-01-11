#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

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

class Animation {
  private:
	const Texture &texture;
	std::vector<Rectangle> frames;
	GameClock::duration timePerFrame;
  GameClock::duration runDuration = GameClock::duration::zero();

  public:
	Animation(const Texture &texture, std::vector<Rectangle> frames, GameClock::duration timePerFrame);
	const std::vector<Rectangle> &getFrames();

	inline int getFrameCount() const
	{
		return frames.size();
	}
  inline GameClock::duration totalDuration(){
    return frames.size() * timePerFrame;
  }
  void reset();
	Sprite updateAnimation(GameClock::duration frameDelta);
  Sprite getCurrentFrame();
	Sprite getSprite(int index) const;
};

} // namespace sdl

#endif /* PRIMITIVES_H */
