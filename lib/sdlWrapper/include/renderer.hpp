#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <memory>

#include "rectangle.hpp"
#include "sdlException.hpp"
#include "sprite.hpp"
#include "texture.hpp"

namespace sdl {
class Renderer {
  private:
	SDL_Renderer *rawRenderer;
	SDL_Window *rawWindow;

  public:
	static const int logicalW = 1920;
	static const int logicalH = 1080;

	Renderer();
	virtual ~Renderer();

	void clear();
	void render(const std::shared_ptr<Texture> texture, Rectangle sourceRect, Rectangle targetRect) const;
	void render(const std::shared_ptr<Texture> texture) const;
	void render(const Sprite &sprite) const;
	void render(const Sprite &sprite, Rectangle targetRect) const;
	void swapBuffers() const;
	SDL_Renderer *getRawRenderer() const;
};
} // namespace sdl
#endif /* RENDERER_H */