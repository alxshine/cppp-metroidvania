#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <chrono>
#include <memory>
#include <string>

#include "events.hpp"
#include "font.hpp"
#include "mixer.hpp"
#include "rendering.hpp"
#include "sdlException.hpp"

namespace sdl {
class SDL final {
  private:
	std::unique_ptr<Renderer> renderer;
	SDL();

  public:
	static SDL &getInstance();
	const Renderer &getRenderer();
	std::unique_ptr<Texture> loadTexture(const std::string &path) const;

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
	std::unique_ptr<Font> loadFont(const std::string &path, unsigned int size);

	std::unique_ptr<SoundEffect> loadSoundEffect(const std::string &path) const;

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
