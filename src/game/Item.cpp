#include "game/Item.hpp"
using namespace game;

Item::Item(const std::string name, Rectangle hitbox, Rectangle renderSize, const sdl::Animation anim)
    : name(name), movable(0), hitbox(hitbox), renderSize(renderSize), animation(anim)
{
}

Item::Item(const Item &rhs)
    : name(rhs.name), movable(rhs.movable), hitbox(rhs.hitbox), renderSize(rhs.renderSize), animation(rhs.animation)
{
}

Item::~Item() {}

void Item::render(const sdl::Renderer &renderer, const sdl::GameClock::time_point &t,
                  const sdl::RenderOptions &options) const
{
	Rectangle destRect = calcRenderTarget();
	Rectangle hitbox = calcPositionedHitbox();

	renderer.render(animation.getAnimationFrame(t), destRect);

	// draw debug info
	if (options.renderEntityDrawRectangles)
		renderer.drawRectangle(destRect, {0, 0, 255, 128}, false);
	if (options.renderHitBoxes)
		renderer.drawRectangle(hitbox, {255, 0, 0, 128}, false);
}

Rectangle game::Item::calcRenderTarget() const
{
	// Calculate position, left-aligning horizontally and bottom-aligning vertically
	return {movable.getPosition().x, movable.getPosition().y - renderSize.h,
	        renderSize.w, renderSize.h};
}

Rectangle Item::calcPositionedHitbox() const
{
	Rectangle destRect = calcRenderTarget(); // Inefficient, for Mob::render(), but maybe good enough
	return {destRect.x + hitbox.x, destRect.y + hitbox.y, hitbox.w, hitbox.h};
}

Door::Door(const std::string name, Item actualDoor, Direction direction, const std::string targetRoom,
           const std::string targetDoorName)
    : name(name), item(actualDoor), direction(direction), targetRoom(targetRoom), targetDoorName(targetDoorName)
{
}

Door::Door(const Door &rhs) : name(rhs.name), item(rhs.item), direction(rhs.direction), targetRoom(rhs.targetRoom), targetDoorName(rhs.targetDoorName) {}
