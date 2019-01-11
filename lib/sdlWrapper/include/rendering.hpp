#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDL.h>
#include <memory>
#include <vector>

#include "Color.hpp"
#include "GameClock.hpp"
#include "SdlException.hpp"
#include "geometry.hpp"
#include "primitives.hpp"

namespace sdl {

class Renderer;
class Renderable;

struct RenderOptions {
	bool renderCollisionMap = false;
	bool renderHitBoxes = false;
	bool renderEntityDrawRectangles = false;
	bool renderHealthBars = false;
};

class Renderable {
  public:
	virtual void render(const Renderer &renderer, GameClock::duration frameDelta,
	                    const RenderOptions &options = {}) = 0;
	virtual ~Renderable(){};
};

class Renderer {
  private:
	SDL_Renderer *rawRenderer;
	SDL_Window *rawWindow;
	mutable Point cameraPosition = {0, 0};
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

	void render(Renderable &renderable, GameClock::duration frameDelta, const RenderOptions &options = {}) const
	{
		renderable.render(*this, frameDelta, options);
	};
	void resetCamera() const;
	void setCameraPosition(Point point) const;
	void render(const Texture &texture, Flip = Flip::None) const;
	void render(const Texture &texture, const Rectangle targetRect, Flip = Flip::None) const;
	void render(const Texture &texture, const Rectangle sourceRect, const Rectangle targetRect,
	            Flip = Flip::None) const;
	void render(const Sprite &sprite, Flip = Flip::None) const;
	void render(const Sprite &sprite, const Rectangle targetRect, Flip = Flip::None) const;
	void render(const Text &text, const Rectangle targetRect) const;
	void render(const Text &text, const Rectangle targetRect, Color color) const;

	void drawRectangle(const Rectangle rect, const Color color = defaultColor, bool fill = true) const;
	void clear(Color color = defaultColor) const;
	void swapBuffers() const;

	SDL_Renderer *getRawRenderer() const;
};
} // namespace sdl
#endif /* RENDERING_H */
