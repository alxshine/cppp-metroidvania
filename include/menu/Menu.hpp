#ifndef MENU_H
#define MENU_H

#include "SDL.hpp"

namespace menu {

class Menu : public sdl::Renderable {
  public:
	virtual void render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
	                    const sdl::RenderOptions &options = {}) override = 0;
	virtual void dispatch() = 0;
	virtual void playMusic() = 0;
	virtual ~Menu(){};
};

} // namespace menu

#endif /* MENU_H */
