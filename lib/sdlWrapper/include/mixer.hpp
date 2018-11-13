#ifndef MIXER_H
#define MIXER_H

#include <SDL2/SDL_mixer.h>
#include <chrono>

#include "SoundEffect.hpp"

namespace sdl {

enum class Channel { any_all = -1, player, mobs, weapons, background };

// Basic

inline void play(Channel channel, const SoundEffect &soundEffect, int loops)
{
	Mix_PlayChannel(static_cast<int>(channel), soundEffect.rawChunk, loops);
};

inline void play(Channel channel, const SoundEffect &soundEffect)
{
	play(channel, soundEffect, 0);
}

inline void pause(Channel channel)
{
	Mix_Pause(static_cast<int>(channel));
};

inline void resume(Channel channel)
{
	Mix_Resume(static_cast<int>(channel));
};

inline void halt(Channel channel)
{
	Mix_HaltChannel(static_cast<int>(channel));
};

// Special

inline void playAtMostFor(Channel channel, const SoundEffect &soundEffect, int loops,
                          std::chrono::milliseconds maxDuration)
{
	Mix_PlayChannelTimed(static_cast<int>(channel), soundEffect.rawChunk, loops, maxDuration.count());
}

inline void playFadeIn(Channel channel, const SoundEffect &soundEffect, int loops,
                       std::chrono::milliseconds maxDuration)
{
	Mix_FadeInChannel(static_cast<int>(channel), soundEffect.rawChunk, loops, maxDuration.count());
}

inline void fadeOut(Channel channel, std::chrono::milliseconds maxDuration)
{
	Mix_FadeOutChannel(static_cast<int>(channel), maxDuration.count());
};

// TODO define volume type once I figure out how to limit to 128
inline void setVolume(Channel channel, unsigned int volume)
{
	Mix_Volume(static_cast<int>(channel), volume);
};

} // namespace sdl
#endif /* MIXER_H */
