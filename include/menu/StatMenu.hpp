#ifndef STATMENU_H
#define STATMENU_H

#include "game/Item.hpp"
#include "game/Mob.hpp"
#include "game/Player.hpp"
#include "menu/Menu.hpp"
#include <set>

namespace game {

class StatMenu : public menu::Menu {
  public:
	StatMenu(const Player& player, std::function<void()> escapeCallback);

	void render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
	            const sdl::RenderOptions &options) override;
	void dispatch() override;
	void playMusic() override;

  private:
	const std::unique_ptr<sdl::Font> titleFont =
	    sdl::SDL::getInstance().loadFont("assets/fonts/Haunted Castle.ttf", 14);
	const std::unique_ptr<sdl::Font> font = sdl::SDL::getInstance().loadFont("assets/fonts/Haunted Castle.ttf", 8);
	// const sdl::Renderer &renderer = sdl::SDL::getInstance().getRenderer();
	sdl::EventHandler eventHandler;

	const sdl::Text title = sdl::SDL::getInstance().generateText(*titleFont, "Stats");
	const int opacity = 125;
	int level;
	int xp;
	int hp;
	int maxHp;
	int threshold;
};

} // namespace game
#endif
