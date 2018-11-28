#include "game/Player.hpp"

using namespace game;

Player::Player(const sdl::Animation idleAnimation, const sdl::Animation walkingAnimation)
    : idleAnimation(idleAnimation), walkingAnimation(walkingAnimation)
{
}

void Player::render(const sdl::Renderer &renderer, const sdl::GameClock::time_point &t,
                    const sdl::RenderOptions &options) const
{
	// Calculate position, centering horizontally but not vertically
	Rectangle destRect{movable.getPosition().x * tileSize.w - tileSize.w,
	                   movable.getPosition().y * tileSize.h - 2 * tileSize.h, tileSize.w * 2, tileSize.h * 2};

	// TODO render hitbox using debug option
	if (movable.getMoved())
		// TODO handle position and direction
		renderer.render(walkingAnimation.getAnimationFrame(t), destRect);
	else
		// TODO handle position and direction
		renderer.render(idleAnimation.getAnimationFrame(t), destRect);
}
