#include "music.hpp"
#include "sdlException.hpp"

using namespace sdl;

Music::~Music()
{
	if (rawMusic != nullptr)
		Mix_FreeMusic(rawMusic);
}

Music::Music(Mix_Music *raw) : rawMusic(raw) {}

Music::Music(Music &&rhs) : rawMusic(rhs.rawMusic)
{
	rhs.rawMusic = nullptr;
};

