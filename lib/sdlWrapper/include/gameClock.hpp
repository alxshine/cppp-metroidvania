#ifndef GAMECLOCK_H
#define GAMECLOCK_H

#include <SDL2/SDL_timer.h>
#include <chrono>

class GameClock {
  public:
	using duration = std::chrono::milliseconds;
	using rep = duration::rep;
	using period = duration::period;
	using time_point = std::chrono::time_point<GameClock>;
	static const bool is_steady = true;

	time_point now() noexcept;
	void pause() noexcept;
	void resume() noexcept;

	GameClock();
  private:
  	time_point nowRaw() noexcept;
  	time_point pauseStart;
  	time_point pauseStartRaw;
  	duration offset;
  	bool paused;
};

#endif /* GAMECLOCK_H */
