#include "Music.hpp"
#include "SdlException.hpp"

using namespace sdl;

const Music *sdl::currentlyPlaying;

Music::~Music()
{
	Mix_FreeMusic(rawMusic);
}

Music::Music(Mix_Music *raw) : rawMusic(raw) {}

Music::Music(Music &&rhs) : rawMusic(rhs.rawMusic)
{
	rhs.rawMusic = nullptr;
};
