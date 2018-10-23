#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <chrono>
#include <memory>
#include <string>

#include "renderer.hpp"

namespace sdl {
class SDL {
private:
  static std::shared_ptr<SDL> singleton;
  std::shared_ptr<Renderer> renderer;
  SDL();

public:
  static SDL &getInstance();
  std::shared_ptr<Renderer> getRenderer();
  std::shared_ptr<Texture> loadTexture(std::string path);
  void delay(std::chrono::milliseconds time);
  virtual ~SDL();
};
} // namespace sdl
#endif /* SDL_H */
