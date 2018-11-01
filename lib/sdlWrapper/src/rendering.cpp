//
#include "rendering.hpp"

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

void sdl::Renderer::drawRectangle(Rectangle rect, Color color, bool fill)
{
	SDL_SetRenderDrawColor(rawRenderer, color.r, color.g, color.b, color.a);
	if (fill)
		SDL_RenderFillRect(rawRenderer, &rect);
	else
		SDL_RenderDrawRect(rawRenderer, &rect);
	SDL_SetRenderDrawColor(rawRenderer, defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a);
}

void sdl::Renderer::clear(Color color)
{
	SDL_SetRenderDrawColor(rawRenderer, color.r, color.g, color.b, color.a);
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

sdl::Texture::~Texture()
{
	if (rawTexture != nullptr)
		SDL_DestroyTexture(rawTexture);
}

sdl::Texture::Texture(SDL_Texture *raw)
{
	rawTexture = raw;
}

void sdl::Texture::changeColor(Color color)
{
	if (SDL_SetTextureColorMod(rawTexture, color.r, color.g, color.b))
		throw SdlException(SDL_GetError());
}

sdl::Animation::Animation(std::shared_ptr<Texture> t, std::vector<sdl::Rectangle> f, sdl::GameClock::duration tpf)
    : texture(t), frames(f), timePerFrame(tpf)
{
}

sdl::Sprite sdl::Animation::getAnimationFrame(sdl::GameClock::time_point t)
{
	int frameNumber = t.time_since_epoch() / timePerFrame;
	int frameIndex = frameNumber % frames.size();
	return Sprite{texture, frames[frameIndex]};
}

