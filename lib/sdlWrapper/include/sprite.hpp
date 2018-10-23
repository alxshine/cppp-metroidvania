#ifndef SPRITE_H
#define SPRITE_H

#include <memory>
#include <rectangle.hpp>
#include <texture.hpp>

namespace sdl {
struct Sprite {
public:
  std::shared_ptr<Texture> texture;
  Rectangle sourceRectangle;
};
} // namespace sdl

#endif /* SPRITE_H */
