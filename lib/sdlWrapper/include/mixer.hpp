#ifndef MIXER_H
#define MIXER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <chrono>

#include "SoundEffect.hpp"
#include "Music.hpp"

namespace sdl {

static constexpr int any_all_channels = -1;
static constexpr int repeat_forever = -1;

// Basic

// 0 loops = play once
inline void play(const SoundEffect &soundEffect, int loops = 0)
{
	Mix_PlayChannel(any_all_channels, soundEffect.rawChunk, loops);
};


// 0 loops = play 0 times (I know, this is retarded!)
inline void play(const Music &music, int loops = 1)
{
	// TODO only play if not already playing the same song, to avoid repeating!
	Mix_PlayMusic(music.rawMusic, loops);
}

// Special

inline void play_fade_out(const SoundEffect &soundEffect, int loops, std::chrono::milliseconds maxDuration)
{
	Mix_PlayChannelTimed(any_all_channels, soundEffect.rawChunk, loops, maxDuration.count());
}

inline void play_fade_in(const SoundEffect &soundEffect, int loops, std::chrono::milliseconds maxDuration)
{
	Mix_FadeInChannel(any_all_channels, soundEffect.rawChunk, loops, maxDuration.count());
}

inline void play_fade_in(const Music &music, int loops, std::chrono::milliseconds fadeIn)
{
	// TODO only play if not already playing the same song, to avoid repeating!
	Mix_FadeInMusic(music.rawMusic, loops, fadeIn.count());
}

namespace sounds {

inline void pause()
{
	Mix_Pause(any_all_channels);
};

inline void resume()
{
	Mix_Resume(any_all_channels);
};

inline void halt()
{
	Mix_HaltChannel(any_all_channels);
};

template<unsigned int volume>
inline void set_volume()
{
	static_assert(volume <= 128, "volume exceeds maximum value");
	Mix_Volume(any_all_channels, volume);
};

inline void fade_out(std::chrono::milliseconds maxDuration)
{
	Mix_FadeOutChannel(any_all_channels, maxDuration.count());
};

} // namespace sounds

namespace music {

inline void pause()
{
	Mix_PauseMusic();
}

inline void resume()
{
	Mix_ResumeMusic();
}

inline void halt()
{
	Mix_HaltMusic();
}

inline void fade_out(std::chrono::milliseconds maxDuration)
{
	Mix_FadeOutMusic(maxDuration.count());
}

template<unsigned int volume>
inline void set_volume()
{
	static_assert(volume <= 128, "volume exceeds maximum value");
	Mix_VolumeMusic(volume);
};


inline bool is_playing()
{
	return Mix_PlayingMusic();
}

inline bool is_paused()
{
	return Mix_PausedMusic();
}

inline bool is_fading()
{
	return Mix_FadingMusic();
}

inline void block_until_stopped()
{
	while (is_playing() && !is_paused())
		SDL_Delay(50);
}

} // namespace music

} // namespace sdl
#endif /* MIXER_H */
