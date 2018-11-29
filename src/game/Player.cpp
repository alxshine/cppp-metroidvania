#include "game/Player.hpp"

using namespace game;

Player::Player(const sdl::Animation idleAnimation, const sdl::Animation walkingAnimation)
    : movable(100), idleAnimation(idleAnimation), walkingAnimation(walkingAnimation)
{
}

void Player::render(const sdl::Renderer &renderer, const sdl::GameClock::time_point &t,
                    const sdl::RenderOptions &options) const
{
	// Calculate position, centering horizontally and bottom-aligning vertically
	Rectangle destRect{movable.getPosition().x - tileSize.w, movable.getPosition().y - 2 * tileSize.h, tileSize.w * 2,
	                   tileSize.h * 2};

	sdl::Renderer::Flip flip;
	switch (movable.getDirection()) {
	case Direction::Up:
		// fallthrough
	case Direction::Down:
		// fallthrough
	case Direction::Right:
		flip = sdl::Renderer::Flip::None;
		break;
	case Direction::Left:
		flip = sdl::Renderer::Flip::X;
		break;
	}

	if (movable.getMoved())
		renderer.render(walkingAnimation.getAnimationFrame(t), destRect, flip);
	else
		renderer.render(idleAnimation.getAnimationFrame(t), destRect, flip);

	// draw texture box
	if (options.renderEntityDrawRectangles)
		renderer.drawRectangle(destRect, {0, 0, 255, 128}, false);
	// draw hit/collision box
	if (options.renderHitBoxes)
		renderer.drawRectangle({destRect.x + hitbox.x, destRect.y + hitbox.y, hitbox.w, hitbox.h}, {255, 0, 0, 128},
		                       false);
}