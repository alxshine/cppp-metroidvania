#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

#include "Color.hpp"
#include "GameClock.hpp"
#include "primitives.hpp"
#include "Rectangle.hpp"
#include "SdlException.hpp"

namespace sdl {

class Renderer;
class Renderable;

class Renderable {
  public:
	virtual void render(const Renderer &renderer, const GameClock::time_point &t) const = 0;
	virtual ~Renderable(){};
};

class Renderer {
  private:
	SDL_Renderer *rawRenderer;
	SDL_Window *rawWindow;
	static constexpr Color defaultColor = {0, 0, 0, 0};

  public:
	static constexpr int logicalW = 1920;
	static constexpr int logicalH = 1080;

	Renderer();
	virtual ~Renderer();

	void render(Renderable &renderable, const GameClock::time_point &t) const
	{
		renderable.render(*this, t);
	};
	// TODO inline as much as possible here -- or not?
	void render(const Texture &texture, const Rectangle sourceRect, const Rectangle targetRect) const;
	void render(const Texture &texture) const;
	void render(const Sprite &sprite) const;
	void render(const Sprite &sprite, const Rectangle targetRect) const;

	void drawRectangle(const Rectangle rect, const Color color = defaultColor, bool fill = true) const;
	void clear(Color color = defaultColor) const;
	void swapBuffers() const;

	SDL_Renderer *getRawRenderer() const;
};
} // namespace sdl
#endif /* RENDERING_H */
