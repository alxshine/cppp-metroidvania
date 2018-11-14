#ifndef MUSIC_H
#define MUSIC_H

#include <SDL2/SDL_mixer.h>

namespace sdl {

struct Music {
	Music(Mix_Music *raw);

	Mix_Music *rawMusic;

	Music() = delete;

	Music(const Music &) = delete;

	Music(Music &&rhs);

	~Music();
};

} // namespace sdl

#endif /* MUSIC_H */
