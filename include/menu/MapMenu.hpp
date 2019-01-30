#ifndef MAPMENU_H
#define MAPMENU_H

#include <algorithm>

#include "game/Player.hpp"
#include "game/ResourceManager.hpp"
#include "menu/Menu.hpp"

namespace game {
class MapMenu : public menu::Menu {
  public:
	MapMenu(std::string currentRoom, const Player &player, const ResourceManager& resourceManager, std::function<void()> escapeCallback);

	virtual void render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
	                    const sdl::RenderOptions &options = {}) override;
	void dispatch() override;
	void playMusic() override;

  private:
	const std::unique_ptr<sdl::Font> titleFont =
	    sdl::SDL::getInstance().loadFont("assets/fonts/Haunted Castle.ttf", 14);
	const std::unique_ptr<sdl::Font> font = sdl::SDL::getInstance().loadFont("assets/fonts/Haunted Castle.ttf", 5);

	sdl::EventHandler eventHandler;

	const sdl::Text title = sdl::SDL::getInstance().generateText(*titleFont, "Map");
	const int opacity = 125;

	std::string currentRoom;
	std::map<std::string, MapRoom> toRender;
	int minX, maxX, minY, maxY;
};
} // namespace game

#endif
