#ifndef MENU_H
#define MENU_H

#include "SDL.hpp"

namespace menu {

class Menu : public sdl::Renderable {
  public:
	//render the menu
	virtual void render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
	                    const sdl::RenderOptions &options = {}) override = 0;
	//called every loop, used to check for key events
	virtual void dispatch() = 0;
	//play the menu music
	virtual void playMusic() = 0;
	virtual ~Menu(){};
};

} // namespace menu

#endif /* MENU_H */
