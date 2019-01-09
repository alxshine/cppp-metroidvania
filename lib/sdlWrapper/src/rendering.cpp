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

	Rectangle newTarget = rect;
	newTarget.x -= cameraPosition.x;
	newTarget.y -= cameraPosition.y;
	SDL_SetRenderDrawColor(rawRenderer, color.r, color.g, color.b, color.a);
	if (fill)
		SDL_RenderFillRect(rawRenderer, &newTarget);
	else
		SDL_RenderDrawRect(rawRenderer, &newTarget);
	SDL_SetRenderDrawColor(rawRenderer, defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a);
}

void sdl::Renderer::clear(Color color) const
{
	SDL_SetRenderDrawColor(rawRenderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(rawRenderer);
}

void sdl::Renderer::render(const Texture &texture, Rectangle sourceRect, Rectangle targetRect, Flip flip) const
{
	Rectangle newTarget = targetRect;
	newTarget.x -= cameraPosition.x;
	newTarget.y -= cameraPosition.y;
	SDL_RenderCopyEx(rawRenderer, texture.rawTexture, &sourceRect, &newTarget, 0, nullptr,
	                 static_cast<SDL_RendererFlip>(flip));
}

void sdl::Renderer::render(const Texture &texture, Flip flip) const
{
	SDL_RenderCopyEx(rawRenderer, texture.rawTexture, nullptr, nullptr, 0, nullptr,
	                 static_cast<SDL_RendererFlip>(flip));
}

void sdl::Renderer::render(const Texture &texture, const Rectangle targetRect, Flip flip) const
{
	Rectangle newTarget = targetRect;
	newTarget.x -= cameraPosition.x;
	newTarget.y -= cameraPosition.y;
	SDL_RenderCopyEx(rawRenderer, texture.rawTexture, nullptr, &newTarget, 0, nullptr,
	                 static_cast<SDL_RendererFlip>(flip));
}

void sdl::Renderer::render(const Sprite &sprite, Flip flip) const
{
	SDL_RenderCopyEx(rawRenderer, sprite.texture.rawTexture, &sprite.sourceRectangle, nullptr, 0, nullptr,
	                 static_cast<SDL_RendererFlip>(flip));
}

void sdl::Renderer::render(const Sprite &sprite, Rectangle targetRect, Flip flip) const
{
	Rectangle newTarget = targetRect;
	newTarget.x -= cameraPosition.x;
	newTarget.y -= cameraPosition.y;
	SDL_RenderCopyEx(rawRenderer, sprite.texture.rawTexture, &sprite.sourceRectangle, &newTarget, 0, nullptr,
	                 static_cast<SDL_RendererFlip>(flip));
}

void sdl::Renderer::render(const Text &text, const Rectangle targetRect) const
{
	Rectangle newTarget = targetRect;
	newTarget.x -= cameraPosition.x;
	newTarget.y -= cameraPosition.y;
	SDL_RenderCopy(rawRenderer, text.texture->rawTexture, &text.sourceRectangle, &newTarget);
}

void sdl::Renderer::render(const Text &text, const Rectangle targetRect, Color color) const
{
	Rectangle newTarget = targetRect;
	newTarget.x -= cameraPosition.x;
	newTarget.y -= cameraPosition.y;
	SDL_SetTextureColorMod(text.texture->rawTexture, color.r, color.g, color.b);
	SDL_RenderCopy(rawRenderer, text.texture->rawTexture, &text.sourceRectangle, &newTarget);

}

SDL_Renderer *sdl::Renderer::getRawRenderer() const
{
	return rawRenderer;
}

void sdl::Renderer::swapBuffers() const
{
	SDL_RenderPresent(rawRenderer);
}

void sdl::Renderer::setCameraPosition(Point point) const
{
	cameraPosition = point;
}
