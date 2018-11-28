#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

#include "Color.hpp"
#include "GameClock.hpp"
#include "Rectangle.hpp"
#include "SdlException.hpp"
#include "primitives.hpp"

namespace sdl {

class Renderer;
class Renderable;

struct RenderOptions {
	bool renderCollisionMap = false;
	bool renderHitBoxes = false;
	bool renderEntityDrawRectangles = false;
};

class Renderable {
  public:
	virtual void render(const Renderer &renderer, const GameClock::time_point &t,
	                    const RenderOptions &options = {}) const = 0;
	virtual ~Renderable(){};
};

class Renderer {
  private:
	SDL_Renderer *rawRenderer;
	SDL_Window *rawWindow;
	static constexpr Color defaultColor = {0, 0, 0, 0};

  public:
	enum class Flip {
		None = 0,
		X = SDL_FLIP_HORIZONTAL,
		Y = SDL_FLIP_VERTICAL,
		XY = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL
	};

	static constexpr int logicalW = 320;
	static constexpr int logicalH = 180;

	Renderer();
	virtual ~Renderer();

	void render(Renderable &renderable, const GameClock::time_point &t, const RenderOptions &options = {}) const
	{
		renderable.render(*this, t, options);
	};
	// TODO inline as much as possible here -- or not?
	void render(const Texture &texture, Flip = Flip::None) const;
	void render(const Texture &texture, const Rectangle targetRect, Flip = Flip::None) const;
	void render(const Texture &texture, const Rectangle sourceRect, const Rectangle targetRect,
	            Flip = Flip::None) const;
	void render(const Sprite &sprite, Flip = Flip::None) const;
	void render(const Sprite &sprite, const Rectangle targetRect, Flip = Flip::None) const;

	void drawRectangle(const Rectangle rect, const Color color = defaultColor, bool fill = true) const;
	void clear(Color color = defaultColor) const;
	void swapBuffers() const;

	SDL_Renderer *getRawRenderer() const;
};
} // namespace sdl
#endif /* RENDERING_H */
