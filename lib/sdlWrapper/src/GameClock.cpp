#include "GameClock.hpp"

sdl::GameClock::GameClock() : offset(std::chrono::milliseconds(SDL_GetTicks())) {}

sdl::GameClock::time_point sdl::GameClock::now() noexcept
{
	if (paused)
		return pauseStart;

	return time_point(nowRaw() - offset);
}

sdl::GameClock::time_point sdl::GameClock::nowRaw() noexcept
{
	return time_point(std::chrono::milliseconds(SDL_GetTicks()));
}

void sdl::GameClock::pause() noexcept
{
	if (!paused) {
		pauseStart = time_point(nowRaw()-offset);
		pauseStart = now();
		pauseStartRaw = nowRaw();
		paused = true;
	}
}

void sdl::GameClock::resume() noexcept
{
	if (paused) {
		offset += nowRaw() - pauseStartRaw;
		paused = false;
	}
}
