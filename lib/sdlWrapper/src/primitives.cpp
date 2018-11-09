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
