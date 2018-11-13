#ifndef SOUNDEFFECT_H
#define SOUNDEFFECT_H

#include <SDL2/SDL_mixer.h>

namespace sdl {

struct SoundEffect {
	SoundEffect(Mix_Chunk *raw);

	Mix_Chunk *rawChunk;

	SoundEffect() = delete;

	SoundEffect(const SoundEffect &) = delete;

	SoundEffect(SoundEffect &&rhs);

	void setVolume(unsigned int volume);

	~SoundEffect();
};

} // namespace sdl

#endif /* SOUNDEFFECT_H */
