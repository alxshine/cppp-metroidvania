//
#include <renderer.hpp>

#include "renderer.hpp"

sdl::Renderer::Renderer()
{
	// Create window
	rawWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1600, 900, SDL_WINDOW_SHOWN);
	if (rawWindow == nullptr)
		throw SdlException("Window could not be created!");

	rawRenderer = SDL_CreateRenderer(rawWindow, -1, SDL_RENDERER_ACCELERATED);
	if (rawRenderer == nullptr)
		throw SdlException("Renderer could not be created");

	SDL_RenderSetLogicalSize(rawRenderer, logicalW, logicalH);
	//	SDL_SetRenderDrawColor(rawRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

sdl::Renderer::~Renderer()
{
	SDL_DestroyRenderer(rawRenderer);
}

void sdl::Renderer::clear()
{
	SDL_RenderClear(rawRenderer);
}

void sdl::Renderer::render(const std::shared_ptr<Texture> texture, Rectangle sourceRect, Rectangle targetRect) const
{
	SDL_RenderCopy(rawRenderer, texture->rawTexture, &sourceRect, &targetRect);
}

SDL_Renderer *sdl::Renderer::getRawRenderer() const
{
	return rawRenderer;
}

void sdl::Renderer::render(const std::shared_ptr<Texture> texture) const
{
	SDL_RenderCopy(rawRenderer, texture->rawTexture, nullptr, nullptr);
}

void sdl::Renderer::render(const Sprite &sprite) const
{
	SDL_RenderCopy(rawRenderer, sprite.texture->rawTexture, &sprite.sourceRectangle, nullptr);
}

void sdl::Renderer::render(const Sprite &sprite, Rectangle targetRect) const
{
	SDL_RenderCopy(rawRenderer, sprite.texture->rawTexture, &sprite.sourceRectangle, &targetRect);
}

void sdl::Renderer::swapBuffers() const
{
	SDL_RenderPresent(rawRenderer);
}
