#include "game/Item.hpp"
using namespace game;

Item::Item(std::string name, std::string description, Rectangle hitbox, Rectangle renderSize, const sdl::Animation anim)
    : name(std::move(name)), description(std::move(description)), movable(hitbox, 0), animation(anim),
      renderSize(renderSize)
{
}

Item::Item(const Item &rhs)
    : name(rhs.name), description(rhs.description), movable(rhs.movable), animation(rhs.animation),
      renderSize(rhs.renderSize)
{
}

Item::~Item() {}

void Item::render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta, const sdl::RenderOptions &options)
{
	Rectangle destRect = calcRenderTarget();
	Rectangle hitbox = movable.calcPositionedHitbox();

	renderer.render(animation.updateAnimation(frameDelta), destRect);

	// draw debug info
	if (options.renderEntityDrawRectangles)
		renderer.drawRectangle(destRect, {0, 0, 255, 128}, false);
	if (options.renderHitBoxes)
		renderer.drawRectangle(hitbox, {0, 255, 0, 128}, false);
}

Rectangle game::Item::calcRenderTarget() const
{
	// Calculate position, left-aligning horizontally and bottom-aligning vertically
	return {movable.getPosition().x - renderSize.w / 2, movable.getPosition().y - renderSize.h, renderSize.w,
	        renderSize.h};
}

Door::Door(const std::string name, Item actualDoor, Direction direction, const std::string targetRoom,
           const std::string targetDoorName)
    : name(name), item(actualDoor), direction(direction), targetRoom(targetRoom), targetDoorName(targetDoorName)
{
}

Door::Door(const Door &rhs)
    : name(rhs.name), item(rhs.item), direction(rhs.direction), targetRoom(rhs.targetRoom),
      targetDoorName(rhs.targetDoorName)
{
}
