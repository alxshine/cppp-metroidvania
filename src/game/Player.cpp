#include "game/Player.hpp"

using namespace game;

Player::Player(const sdl::Animation idleAnimation, const sdl::Animation walkingAnimation,
               const sdl::Animation airUpAnimation, const sdl::Animation airDownAnimation,
               const std::vector<Attack> attacks)
    : movable(100, walkingAnimation, airUpAnimation, airDownAnimation), attackable(100, attacks, *this),
      idleAnimation(idleAnimation)
{
}

Rectangle adjustByHitBox(Position position)
{
	return {position.x - static_cast<int>(tileSize.w / 2), position.y - 2 * tileSize.h, tileSize.w, tileSize.h * 2};
}

Rectangle Player::calcPositionedHitbox() const
{
	return adjustByHitBox(movable.getPosition());
}

Rectangle Player::calcLastPositionedHitbox() const
{
	return adjustByHitBox(movable.getLastPosition());
}

Rectangle Player::calcRenderTarget() const
{

	// Calculate position, centering horizontally and bottom-aligning vertically
	return {movable.getPosition().x - tileSize.w, movable.getPosition().y - 2 * tileSize.h, tileSize.w * 2,
	        tileSize.h * 2};
}

void Player::render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
                    const sdl::RenderOptions &options)
{
	Rectangle destRect = calcRenderTarget();

	sdl::Renderer::Flip flip;
	auto dir = movable.getDirection();
	if (dir.x < 0)
		flip = sdl::Renderer::Flip::X;
	else
		flip = sdl::Renderer::Flip::None;

  if(attackable.isAttacking())
    renderer.render(attackable.getCurrentSprite(), destRect, flip);
	else if (movable.hasPlayableAnimation())
		renderer.render(movable.updateAnimation(frameDelta), destRect, flip);
	else
		renderer.render(idleAnimation.updateAnimation(frameDelta), destRect, flip);

	// draw texture box
	if (options.renderEntityDrawRectangles)
		renderer.drawRectangle(destRect, {0, 0, 255, 128}, false);
	// draw hit/collision box
	if (options.renderHitBoxes)
		renderer.drawRectangle(calcPositionedHitbox(), {255, 0, 0, 128}, false);
}
