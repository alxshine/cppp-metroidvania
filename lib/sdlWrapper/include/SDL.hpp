#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <chrono>
#include <memory>
#include <string>

#include "font.hpp"
#include "renderer.hpp"
#include "sdlException.hpp"

namespace sdl {
class SDL final {
  private:
	static std::shared_ptr<SDL> singleton;
	std::shared_ptr<Renderer> renderer;
	SDL();

  public:
	static SDL &getInstance();
	std::shared_ptr<Renderer> getRenderer();
	std::shared_ptr<Texture> loadTexture(std::string path);

	/**
	 * @brief Load a .ttf font from disk.
	 *
	 * This generates a bitmap font.
	 *
	 * @param path The path to the font file
	 * @param size The point-size to generate
	 *
	 * @return A shared pointer to the generated font.
	 */
	std::shared_ptr<Font> loadFont(std::string path, unsigned int size);

	/**
	 * @brief Draw text to a sprite.
	 *
	 * @param font The font to use
	 * @param text The text to draw
	 * @param color The text color
	 * @param rendering Text rendering method
	 * @param bgColor The background color, only used for TextRendering::Shaded
	 *
	 * @return The generated sprite, ready to be rendered.
	 */
	Text generateText(std::shared_ptr<Font> font, std::string text,
	                                   Color color = {u_char(255), u_char(255), u_char(255), u_char(255)},
	                                   TextRendering rendering = TextRendering::Blended,
	                                   Color bgColor = {u_char(0), u_char(0), u_char(0), u_char(0)});

	void delay(std::chrono::milliseconds time);

	SDL(const SDL &) = delete;
	SDL(const SDL &&) = delete;
	~SDL();
};
} // namespace sdl
#endif /* SDL_H */