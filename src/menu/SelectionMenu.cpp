#include "menu/SelectionMenu.hpp"

using namespace menu;
using namespace sdl;

SelectionMenu::SelectionMenu(std::string title, std::vector<RawMenuItem> items,
                             std::optional<std::reference_wrapper<const sdl::Music>> music,
                             std::function<void()> escapeCallback, int opacity)
    : title(SDL::getInstance().generateText(*titleFont, title)), music(music), opacity(opacity)
{
	playMusic();

	for (auto i : items) {
		this->items.emplace_back(SDL::getInstance().generateText(*font, i.first), i.second);
	}

	eventHandler.onKeyDown(SDLK_DOWN, [&](const KeyboardEvent &) {
		if (selectedItem < this->items.size() - 1)
			selectedItem++;
	});
	eventHandler.onKeyDown(SDLK_UP, [&](const KeyboardEvent &) {
		if (selectedItem > 0)
			--selectedItem;
	});
	eventHandler.onKeyDown(SDLK_RETURN, [&](const KeyboardEvent &) {
		if (selectedItem < this->items.size())
			this->items[selectedItem].second();
	});
	eventHandler.onKeyDown(SDLK_ESCAPE, [=](const KeyboardEvent &) { escapeCallback(); });
}

void SelectionMenu::playMusic()
{
	if (music)
		play(*music);
}

void SelectionMenu::render(const Renderer &renderer, GameClock::duration, const RenderOptions &)
{
	const int margin_x = 30;
	const int padding_y = 10;
	int y = 30;

	// render background
	renderer.drawRectangle({0, 0, renderer.logicalW, renderer.logicalH},
	                       {0, 0, 0, static_cast<unsigned char>(opacity)});

	// render title
	auto target = title.sourceRectangle;
	target.x = margin_x;
	target.y = y;
	y += title.sourceRectangle.h + padding_y;
	renderer.render(title, target);

	// render items
	for (unsigned long i = 0; i < items.size(); ++i) {
		auto currentItem = items[i].first;

		auto fg = selectedItem == i ? Color{0, 0, 0, 255} : Color{255, 255, 255, 255};
		auto bg = selectedItem == i ? Color{255, 255, 255, 255} : Color{0, 0, 0, 0};

		auto target = currentItem.sourceRectangle;
		target.x = margin_x;
		target.y = y;
		y += currentItem.sourceRectangle.h + padding_y;
		renderer.drawRectangle(target, bg);
		target.x += 1;
		target.y += 1;
		renderer.render(currentItem, target, fg);
	}
}

void SelectionMenu::dispatch()
{
	eventHandler.dispatch();
}
