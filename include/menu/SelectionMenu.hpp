#ifndef SELECTIONMENU_H
#define SELECTIONMENU_H

#include "menu/Menu.hpp"

#include <functional>
#include <initializer_list>
#include <optional>
#include <utility>
#include <vector>

namespace menu {

using RawMenuItem = std::pair<std::string, std::function<void()>>;
using MenuItem = std::pair<sdl::Text, std::function<void()>>;

class SelectionMenu : public Menu {
  public:
	SelectionMenu(std::string title, std::initializer_list<RawMenuItem> items,
	              std::optional<std::reference_wrapper<const sdl::Music>> music = std::nullopt,
	              std::function<void()> escapeCallback = []() {});
	void render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
	            const sdl::RenderOptions &options) override;
	void dispatch() override;
	void playMusic() override;
	virtual ~SelectionMenu(){};

  private:
	const std::unique_ptr<sdl::Font> titleFont =
	    sdl::SDL::getInstance().loadFont("assets/fonts/Haunted Castle.ttf", 14);
	const std::unique_ptr<sdl::Font> font = sdl::SDL::getInstance().loadFont("assets/fonts/Haunted Castle.ttf", 12);
	const sdl::Renderer &renderer = sdl::SDL::getInstance().getRenderer();
	sdl::EventHandler eventHandler;

	const sdl::Text title;
	const std::optional<std::reference_wrapper<const sdl::Music>> music;
	std::vector<MenuItem> items;
	unsigned long selectedItem = 0;
};

} // namespace menu

#endif /* SELECTIONMENU_H */
