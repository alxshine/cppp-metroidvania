//
#include "rendering.hpp"

sdl::Renderer::Renderer()
{
	// Create window
	rawWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1600, 900, SDL_WINDOW_SHOWN);
	if (rawWindow == nullptr)
		throw SdlException("Window could not be created!");

	rawRenderer = SDL_CreateRenderer(rawWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (rawRenderer == nullptr) {
		SDL_DestroyWindow(rawWindow);
		throw SdlException("Renderer could not be created");
	}

	SDL_RenderSetLogicalSize(rawRenderer, logicalW, logicalH);
	SDL_SetRenderDrawBlendMode(rawRenderer, SDL_BLENDMODE_BLEND);
	//	SDL_SetRenderDrawColor(rawRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

sdl::Renderer::~Renderer()
{
	SDL_DestroyRenderer(rawRenderer);
	SDL_DestroyWindow(rawWindow);
}

void sdl::Renderer::drawRectangle(Rectangle rect, Color color, bool fill) const
{
	SDL_SetRenderDrawColor(rawRenderer, color.r, color.g, color.b, color.a);
	if (fill)
		SDL_RenderFillRect(rawRenderer, &rect);
	else
		SDL_RenderDrawRect(rawRenderer, &rect);
	SDL_SetRenderDrawColor(rawRenderer, defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a);
}

void sdl::Renderer::clear(Color color) const
{
	SDL_SetRenderDrawColor(rawRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(rawRenderer);
}

void sdl::Renderer::render(const Texture &texture, Rectangle sourceRect, Rectangle targetRect) const
{
	SDL_RenderCopy(rawRenderer, texture.rawTexture, &sourceRect, &targetRect);
}

SDL_Renderer *sdl::Renderer::getRawRenderer() const
{
	return rawRenderer;
}

void sdl::Renderer::render(const Texture &texture) const
{
	SDL_RenderCopy(rawRenderer, texture.rawTexture, nullptr, nullptr);
}

void sdl::Renderer::render(const Texture &texture, const Rectangle targetRect) const{
	SDL_RenderCopy(rawRenderer, texture.rawTexture, nullptr, &targetRect);
}
void sdl::Renderer::render(const Sprite &sprite) const
{
	SDL_RenderCopy(rawRenderer, sprite.texture.rawTexture, &sprite.sourceRectangle, nullptr);
}

void sdl::Renderer::render(const Sprite &sprite, Rectangle targetRect) const
{
	SDL_RenderCopy(rawRenderer, sprite.texture.rawTexture, &sprite.sourceRectangle, &targetRect);
}

void sdl::Renderer::swapBuffers() const
{
	SDL_RenderPresent(rawRenderer);
}
