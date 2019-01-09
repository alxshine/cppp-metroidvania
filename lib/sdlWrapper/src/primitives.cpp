#include "primitives.hpp"

sdl::Texture::~Texture()
{
	SDL_DestroyTexture(rawTexture);
}

sdl::Texture::Texture(SDL_Texture *raw) : rawTexture(raw) {}

sdl::Texture::Texture(Texture &&rhs) : rawTexture(rhs.rawTexture)
{
	rhs.rawTexture = nullptr;
}

sdl::Sprite sdl::Animation::getAnimationFrame(sdl::GameClock::time_point t) const
{
	int frameNumber = t.time_since_epoch() / timePerFrame;
	int frameIndex = frameNumber % frames.size();
	return Sprite{texture, frames[frameIndex]};
}

sdl::Sprite sdl::Animation::getSprite(int index) const{
  int frameCount = frames.size();
  auto i = std::clamp(index, 0, frameCount-1);
  return Sprite{texture, frames[i]};
}

sdl::Animation::Animation(const Texture &t, std::vector<sdl::Rectangle> f, sdl::GameClock::duration tpf)
    : texture(t), frames(f), timePerFrame(tpf)
{
}
