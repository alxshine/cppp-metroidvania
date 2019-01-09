#include "game/Entity.hpp"

using namespace game;

Entity::Entity(const sdl::Animation idleAnimation, const sdl::Animation walkingAnimation)
    : movable(100, walkingAnimation), idleAnimation(idleAnimation)
{
}

Rectangle adjustByHitBox(Position position)
{
	return {position.x - static_cast<int>(tileSize.w / 2), position.y - 2 * tileSize.h, tileSize.w, tileSize.h * 2};
}

Rectangle Entity::calcPositionedHitbox() const
{
	return adjustByHitBox(movable.getPosition());
}

Rectangle Entity::calcLastPositionedHitbox() const
{
	return adjustByHitBox(movable.getLastPosition());
}

Rectangle Entity::calcRenderTarget() const
{

	// Calculate position, centering horizontally and bottom-aligning vertically
	return {movable.getPosition().x - tileSize.w, movable.getPosition().y - 2 * tileSize.h, tileSize.w * 2,
	        tileSize.h * 2};
}

void Entity::render(const sdl::Renderer &renderer, const sdl::GameClock::time_point &t,
                    const sdl::RenderOptions &options) const
{
	Rectangle destRect = calcRenderTarget();

	sdl::Renderer::Flip flip;
	auto dir = movable.getDirection();
	if (dir.x < 0)
		flip = sdl::Renderer::Flip::X;
	else
		flip = sdl::Renderer::Flip::None;

	if (movable.hasPlayableAnimation())
		renderer.render(movable.getAnimationFrame(t), destRect, flip);
	else
		renderer.render(idleAnimation.getAnimationFrame(t), destRect, flip);

	// draw texture box
	if (options.renderEntityDrawRectangles)
		renderer.drawRectangle(destRect, {0, 0, 255, 128}, false);
	// draw hit/collision box
	if (options.renderHitBoxes)
		renderer.drawRectangle(calcPositionedHitbox(), {255, 0, 0, 128}, false);
}
