#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <functional>
#include <iostream>
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
	std::reference_wrapper<const Texture> texture;
	Rectangle sourceRectangle;
};

struct Text final {
  public:
	std::shared_ptr<Texture> texture;
	Rectangle sourceRectangle;
};

class Animation {
  private:
	std::reference_wrapper<const Texture> texture;
	std::vector<Rectangle> frames;
	GameClock::duration timePerFrame;
	GameClock::duration runDuration = GameClock::duration::zero();
	int loopCount = 0;

  public:
	Animation(const Texture &texture, std::vector<Rectangle> frames, GameClock::duration timePerFrame);
	const std::vector<Rectangle> &getFrames();

	inline int getFrameCount() const
	{
		return frames.size();
	}
	inline GameClock::duration totalDuration()
	{
		return frames.size() * timePerFrame;
	}
  inline GameClock::duration getTimePerFrame() const{
    return timePerFrame;
  }
	void reset();
	Sprite updateAnimation(GameClock::duration frameDelta);
	Sprite getCurrentFrame();
	Sprite getSprite(int index) const;
	inline int getLoopCount() const
	{
		return loopCount;
	}
};

} // namespace sdl

#endif /* PRIMITIVES_H */
