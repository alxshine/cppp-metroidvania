#include "menu.hpp"

constexpr int hpadding = 20;
constexpr int vpadding = 6;
constexpr int lmargin = 10;
constexpr int vmargin = 30;

sdl::MenuItem::MenuItem(Text buttonText, std::function<void(void)> onClick) : buttonText(buttonText), onClick(onClick)
{
}

sdl::Menu::Menu(Text titleText, Color backgroundColor, std::shared_ptr<Renderer> renderer)
    : renderer(renderer),
      titleText(titleText), titleDrawArea{renderer->logicalW / 2 - titleText.sourceRectangle.w / 2, vpadding,
                                          titleText.sourceRectangle.w, titleText.sourceRectangle.h},
      backgroundColor(backgroundColor)
{
}

sdl::Menu::~Menu()
{ /* TODO*/
}

sdl::Menu &sdl::Menu::addItem(MenuItem item)
{
	item.buttonArea.x = lmargin;
	item.buttonArea.w = item.buttonText.sourceRectangle.w + hpadding;
	item.buttonArea.h = item.buttonText.sourceRectangle.h + vpadding;
	if (items.empty()) {
		item.buttonArea.y = titleDrawArea.y + titleDrawArea.h + vmargin;
	} else {
		item.buttonArea.y = items.back().buttonArea.y + items.back().buttonArea.h + vmargin;
	}

	item.buttonTextDrawArea = item.buttonText.sourceRectangle;
	item.buttonTextDrawArea.x = item.buttonArea.x + hpadding / 2;
	item.buttonTextDrawArea.y = item.buttonArea.y + vpadding / 2;

	items.push_back(item);
	return *this;
}

void sdl::Menu::render() const
{
	// TODO use background color
	renderer->render(titleText, titleDrawArea);

	for (auto item : items) {
		renderer->render(item.buttonText, item.buttonTextDrawArea);
		// TODO render rectangle in buttonArea;
	}
}
