#include "SoundEffect.hpp"
#include "SdlException.hpp"

using namespace sdl;

SoundEffect::~SoundEffect()
{
	if (rawChunk != nullptr)
		Mix_FreeChunk(rawChunk);
}

SoundEffect::SoundEffect(Mix_Chunk *raw) : rawChunk(raw) {}

SoundEffect::SoundEffect(SoundEffect &&rhs) : rawChunk(rhs.rawChunk)
{
	rhs.rawChunk = nullptr;
};

void SoundEffect::setVolume(unsigned int volume)
{
	Mix_VolumeChunk(rawChunk, volume);
}
