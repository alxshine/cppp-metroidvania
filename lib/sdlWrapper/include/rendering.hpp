#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include <memory>

#include "color.hpp"
#include "rectangle.hpp"
#include "sdlException.hpp"

namespace sdl {
struct Texture {
	Texture(SDL_Texture *raw);

	SDL_Texture *rawTexture; // TODO: maybe we can do this better, but SDL_RenderCopy takes a non const SDL_Texture*

	void changeColor(Color color);

	Texture() = delete;

	Texture(const Texture &) = delete;

	Texture(const Texture &&) = delete;

	virtual ~Texture();
};

struct Sprite {
  public:
	std::shared_ptr<Texture> texture;
	Rectangle sourceRectangle;
};

using Text = Sprite;
class Renderer {
  private:
	SDL_Renderer *rawRenderer;
	SDL_Window *rawWindow;
	static constexpr Color defaultColor = {0, 0, 0, 0};

  public:
	static const int logicalW = 1920;
	static const int logicalH = 1080;

	Renderer();
	virtual ~Renderer();

	void drawRectangle(Rectangle rect, Color color = defaultColor, bool fill = true);
	void clear(Color color = defaultColor);
	void render(const std::shared_ptr<Texture> texture, Rectangle sourceRect, Rectangle targetRect) const;
	void render(const std::shared_ptr<Texture> texture) const;
	void render(const Sprite &sprite) const;
	void render(const Sprite &sprite, Rectangle targetRect) const;
	void swapBuffers() const;
	SDL_Renderer *getRawRenderer() const;
};
} // namespace sdl
#endif /* RENDERER_H */
