#ifndef EVENTS_H
#define EVENTS_H

#include "SDL2/SDL_events.h"

#include <functional>
#include <unordered_map>
#include <map>

namespace sdl {

using KeyboardEvent = SDL_KeyboardEvent;
using Event = SDL_Event;

using Keycode = SDL_Keycode;
using Scancode = SDL_Scancode;
using EventType = SDL_EventType;

class EventHandler final {
  public:
	void dispatch() const;
	void onKeyDown(Keycode key, std::function<void(const KeyboardEvent &)> callback);
	void whileKeyHeld(Scancode key, std::function<void()> callback);
	void on(EventType type, std::function<void(const Event &)> callback);

  private:
	std::unordered_map<Keycode, std::function<void(const KeyboardEvent &)>> keyDownCallbacks;
	std::map<Scancode, std::function<void()>> keyHeldCallbacks; // NOTE: a vector of tuples may be more optimal here....
	std::unordered_map<EventType, std::function<void(const Event &)>> generalCallbacks;
};

} // namespace sdl

#endif /* EVENTS_H */
