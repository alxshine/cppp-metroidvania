#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <chrono>
#include <memory>
#include <string>

#include "font.hpp"
#include "rendering.hpp"
#include "sdlException.hpp"

namespace sdl {
class SDL final {
  private:
	std::shared_ptr<Renderer> renderer;
	SDL();

  public:
	static SDL &getInstance();
	std::shared_ptr<Renderer> getRenderer();
	std::shared_ptr<Texture> loadTexture(const std::string &path);

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
	std::shared_ptr<Font> loadFont(const std::string &path, unsigned int size);

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
	/*
	 *Text generateText(const Font &font, const std::string &text, Color color = {255, 255, 255, 255},
	 *                  TextRendering rendering = TextRendering::Blended, Color bgColor = {0, 0, 0, 0});
	 */

	void delay(std::chrono::milliseconds time);

	SDL(const SDL &) = delete;
	SDL(const SDL &&) = delete;
	~SDL();
};
} // namespace sdl
#endif /* SDL_H */
