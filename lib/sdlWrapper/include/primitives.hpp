#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include <string>
#include <vector>

#include "color.hpp"
#include "rectangle.hpp"
#include "gameClock.hpp"
#include "sdlException.hpp"

namespace sdl {
struct Texture final {
	Texture(SDL_Texture *raw);

	SDL_Texture *rawTexture; // TODO: maybe we can do this better, but SDL_RenderCopy takes a non const SDL_Texture*

	void changeColor(Color color);

	Texture() = delete;

	Texture(const Texture &) = delete;

	Texture(Texture &&rhs); // TODO should the move ctor take const?

	~Texture();
};

struct Sprite final {
  public:
	std::shared_ptr<Texture> texture;
	Rectangle sourceRectangle;
};

using Text = Sprite;

struct Animation {
  private:
	std::shared_ptr<Texture> texture;
	std::vector<Rectangle> frames;
	GameClock::duration timePerFrame;

  public:
	Animation(std::shared_ptr<Texture> texture, std::vector<Rectangle> frames, GameClock::duration timePerFrame);

	Sprite getAnimationFrame(GameClock::time_point t);
};

} // namespace sdl

#endif /* TEXTURE_H */
