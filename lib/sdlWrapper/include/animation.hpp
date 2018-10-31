#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>

#include "gameClock.hpp"
#include "sprite.hpp"

namespace sdl {
struct Animation {
  private:
	std::shared_ptr<Texture> texture;
	std::vector<Rectangle> frames;
	GameClock::duration timePerFrame;

  public:
	Animation(std::shared_ptr<Texture> texture, std::vector<Rectangle> frames, GameClock::duration timePerFrame);
	virtual ~Animation();

	Sprite getAnimationFrame(GameClock::time_point t);
};
} // namespace sdl
#endif /* ANIMATION_H */
