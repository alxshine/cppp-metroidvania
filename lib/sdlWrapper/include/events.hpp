#ifndef EVENTS_H
#define EVENTS_H

#include "SDL2/SDL_events.h"

#include <functional>
#include <unordered_map>

namespace sdl {

using KeyboardEvent = SDL_KeyboardEvent;
using Event = SDL_Event;

using Keycode = SDL_Keycode;
using EventType = SDL_EventType;

class EventHandler final {
  public:
	void dispatch() const;
	void on(Keycode key, std::function<void(const KeyboardEvent &)> callback);
	void on(EventType type, std::function<void(const Event &)> callback);

  private:
	std::unordered_map<Keycode, std::function<void(const KeyboardEvent &)>> keyCallbacks;
	std::unordered_map<EventType, std::function<void(const Event &)>> generalCallbacks;
};

} // namespace sdl

#endif /* EVENTS_H */
