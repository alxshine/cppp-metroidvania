/*
 *#ifndef SDL_MENU_H
 *#define SDL_MENU_H
 *
 *#include <functional>
 *#include <memory>
 *#include <vector>
 *
 *#include "Color.hpp"
 *#include "Rectangle.hpp"
 *#include "rendering.hpp"
 *
 *namespace sdl {
 *
 *class MenuItem {
 *    friend class Menu;
 *
 *  public:
 *    MenuItem(Text buttonText, std::function<void(void)> onClick, Color backgroundColor = {0, 0, 0, 0});
 *    Text buttonText;
 *    Color backgroundColor;
 *    std::function<void(void)> onClick; // TODO we will need to figure out a way to create handle menus in the event
 *                                       // loop! std::optional<Text> labelText;
 *
 *  private:
 *    Rectangle buttonArea;
 *    Rectangle buttonTextDrawArea;
 *    // Rectangle labelArea;
 *};
 *
 *class Menu : public Renderable {
 *    Text titleText;
 *    Rectangle titleDrawArea;
 *    Color backgroundColor;
 *
 *    std::vector<MenuItem> items;
 *
 *  public:
 *    Menu(Text titleText, Color backgroundColor, const std::shared_ptr<Renderer> renderer);
 *
 *    void render(const Renderer &renderer) const;
 *    Menu &addItem(MenuItem item);
 *    virtual ~Menu();
 *};
 *
 *} // namespace sdl
 *
 *#endif [> SDL_MENU_H <]
 */
