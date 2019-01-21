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

void sdl::Animation::reset()
{
	runDuration = sdl::GameClock::duration::zero();
  loopCount = 0;
}

sdl::Sprite sdl::Animation::updateAnimation(sdl::GameClock::duration frameDelta)
{
	runDuration += frameDelta;
	int frameNumber = runDuration.count() / timePerFrame.count();
	int frameIndex = frameNumber % frames.size();
	loopCount =
	    (frameNumber + 1) / frames.size(); // I add +1 here because we want the animation to end after the last frame
	return Sprite{texture, frames[frameIndex]};
}

sdl::Sprite sdl::Animation::getCurrentFrame()
{
	return updateAnimation(sdl::GameClock::duration::zero());
}

sdl::Sprite sdl::Animation::getSprite(int index) const
{
	int frameCount = frames.size();
	auto i = std::clamp(index, 0, frameCount - 1);
	return Sprite{texture, frames[i]};
}

sdl::Animation::Animation(const Texture &t, std::vector<sdl::Rectangle> f, sdl::GameClock::duration tpf)
    : texture(t), frames(f), timePerFrame(tpf)
{
}
