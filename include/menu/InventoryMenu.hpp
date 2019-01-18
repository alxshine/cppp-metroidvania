#ifndef INVENTORYMENU_H
#define INVENTORYMENU_H

#include "game/Item.hpp"
#include "menu/Menu.hpp"
#include <set>

namespace game {

class InventoryMenu : public menu::Menu {
  public:
	InventoryMenu(std::set<Item> inventory, std::function<void()> escapeCallback);

	void render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
	            const sdl::RenderOptions &options) override;
	void dispatch() override;
	void playMusic() override;

  private:
	const std::unique_ptr<sdl::Font> titleFont =
	    sdl::SDL::getInstance().loadFont("assets/fonts/Haunted Castle.ttf", 14);
	const std::unique_ptr<sdl::Font> font = sdl::SDL::getInstance().loadFont("assets/fonts/Haunted Castle.ttf", 5);
	// const sdl::Renderer &renderer = sdl::SDL::getInstance().getRenderer();
	sdl::EventHandler eventHandler;

	const sdl::Text title = sdl::SDL::getInstance().generateText(*titleFont, "Inventory");
	const int opacity = 125;
	std::set<Item> items;
};

} // namespace game
#endif /* INVENTORYMENU_H */
