#include "gameClock.hpp"

GameClock::GameClock() : offset(std::chrono::milliseconds(SDL_GetTicks())), paused(false) {}

GameClock::time_point GameClock::now() noexcept
{
	if (paused)
		return pauseStart;

	return time_point(nowRaw() - offset);
}

GameClock::time_point GameClock::nowRaw() noexcept
{
	return time_point(std::chrono::milliseconds(SDL_GetTicks()));
}

void GameClock::pause() noexcept
{
	if (!paused) {
		pauseStart = time_point(nowRaw()-offset);
		pauseStart = now();
		pauseStartRaw = nowRaw();
		paused = true;
	}
}

void GameClock::resume() noexcept
{
	if (paused) {
		offset += nowRaw() - pauseStartRaw;
		paused = false;
	}
}
