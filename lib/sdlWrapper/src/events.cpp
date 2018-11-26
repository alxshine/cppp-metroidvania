#include "events.hpp"

void sdl::EventHandler::dispatch() const
{
	Event e;
	while (SDL_PollEvent(&e)) {
		// TODO if we find out that we only need SDL_PRESSED, filter for that right here!
		if (e.type == SDL_KEYDOWN) {
			const auto key = e.key.keysym.sym;
			if (keyDownCallbacks.count(key))
				keyDownCallbacks.at(key)(e.key);

		} else {
			const auto type = static_cast<EventType>(e.type);
			if (generalCallbacks.count(type))
				generalCallbacks.at(type)(e);
		}
	}
}

void sdl::EventHandler::on(EventType type, std::function<void(const Event &)> callback)
{
	generalCallbacks.emplace(type, callback);
}

void sdl::EventHandler::onKeyDown(Keycode key, std::function<void(const KeyboardEvent &)> callback)
{
	keyDownCallbacks.emplace(key, callback);
}
