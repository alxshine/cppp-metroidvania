#include "primitives.hpp"

sdl::Texture::~Texture()
{
	if (rawTexture != nullptr)
		SDL_DestroyTexture(rawTexture);
}

sdl::Texture::Texture(SDL_Texture *raw) : rawTexture(raw) {}

sdl::Texture::Texture(Texture &&rhs) : rawTexture(rhs.rawTexture)
{
	rhs.rawTexture = nullptr;
}

void sdl::Texture::changeColor(Color color)
{
	if (SDL_SetTextureColorMod(rawTexture, color.r, color.g, color.b) || SDL_SetTextureAlphaMod(rawTexture, color.a))
		throw SdlException(SDL_GetError());
}

sdl::Sprite sdl::Animation::getAnimationFrame(sdl::GameClock::time_point t)
{
	int frameNumber = t.time_since_epoch() / timePerFrame;
	int frameIndex = frameNumber % frames.size();
	return Sprite{texture, frames[frameIndex]};
}

sdl::Animation::Animation(std::shared_ptr<Texture> t, std::vector<sdl::Rectangle> f, sdl::GameClock::duration tpf)
    : texture(t), frames(f), timePerFrame(tpf)
{
}
