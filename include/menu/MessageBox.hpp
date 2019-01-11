#ifndef MESSAGEBOX_H
#define MESSAGEBOX_H

#include <string>

#include "menu/Menu.hpp"

namespace menu {

class MessageBox : public Menu {
  public:
	MessageBox(std::function<void()> action, std::string line1, std::string line2 = "", std::string line3 = "");
	void render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
	            const sdl::RenderOptions &options) override;
	void dispatch() override;
	void playMusic() override{};

  private:
	const std::unique_ptr<sdl::Font> font = sdl::SDL::getInstance().loadFont("assets/fonts/Haunted Castle.ttf", 8);
	sdl::EventHandler eventHandler;

	sdl::Text line1;
	sdl::Text line2;
	sdl::Text line3;
	int lineCount = 1;
	const int opacity = 180;
};

} // namespace menu

#endif /* MESSAGEBOX_H */
