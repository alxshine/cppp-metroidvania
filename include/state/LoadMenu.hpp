#ifndef LOADMENU_H
#define LOADMENU_H

#include "game/Game.hpp"
#include "menu/SelectionMenu.hpp"
#include "state/SerializedState.hpp"

#include <filesystem>
#include <fstream>
#include <memory>

namespace game {
template <class T>
std::shared_ptr<menu::Menu> createLoadMenu(T &game, std::filesystem::path path)
{
	using namespace std::chrono_literals;
	std::vector<menu::RawMenuItem> menuItems;

	for (auto &f : std::filesystem::directory_iterator(path)) {
		if (!f.is_regular_file())
			continue;
		std::string ext{f.path().extension()};

		if (ext == ".save" + std::to_string(SerializedState::compatibleVersion)) {
			std::cout << "Deserializing " << f.path() << std::endl;
			std::fstream fin(f.path(), std::ios::in);
			SerializedState state;
			fin >> state;

			menuItems.push_back({f.path().stem(), [=,&game]() {
				                     sdl::music::fade_out(500ms);
				                     game.loadState(state);
				                     while (!game.menuStack.empty())
					                     game.menuStack.pop();
			                     }});
		}
	}
	return std::make_shared<menu::SelectionMenu>("Load Game", menuItems, std::nullopt, [&]() { game.menuStack.pop(); });
}
} // namespace game

#endif /* LOADMENU_H */
