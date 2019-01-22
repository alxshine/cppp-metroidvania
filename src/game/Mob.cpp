#include "game/Mob.hpp"
game::Mob::Mob(const Mob &rhs)
    : name(rhs.name), movable(rhs.movable), attackable(rhs.attackable), renderSize(rhs.renderSize),
      walkingAnimation(rhs.walkingAnimation), idleAnimation(rhs.idleAnimation), ai(rhs.ai)
{
}

game::Mob::Mob(const std::string name, Health health, int poise, sdl::GameClock::duration invulnerabilityWindow,
               int speedPerSecond, Rectangle hitbox, Rectangle renderSize, sdl::Animation walkingAnimation,
               sdl::Animation deathAnimation, sdl::Animation hurtAnimation, sdl::Animation idleAnimation,
               std::vector<Attack> attacks, std::shared_ptr<AI> ai)
    : name(name), movable(hitbox, speedPerSecond, walkingAnimation),
      attackable(health, poise, attacks, deathAnimation, hurtAnimation, invulnerabilityWindow), renderSize(renderSize),
      walkingAnimation(std::move(walkingAnimation)), idleAnimation(std::move(idleAnimation)), ai(ai)
{
}

game::Mob::~Mob() {}

game::Rectangle game::Mob::calcRenderTarget() const
{
	// Calculate position, centering horizontally and bottom-aligning vertically
	return {movable.getPosition().x - static_cast<int>(0.5 * renderSize.w), movable.getPosition().y - renderSize.h,
	        renderSize.w, renderSize.h};
}

void game::Mob::performAiStep(const CollisionMap &collisionMap, Rectangle playerHitBox)
{
	if (attackable.hp > 0)
		ai->controlEntity(movable, attackable, collisionMap, playerHitBox);
}

void game::Mob::render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
                       const sdl::RenderOptions &options)
{
	Rectangle destRect = calcRenderTarget();
	Rectangle hitbox = movable.calcPositionedHitbox();

	sdl::Renderer::Flip flip;
	auto dir = movable.getDirection();
	if (dir.x < 0)
		flip = sdl::Renderer::Flip::X;
	else
		flip = sdl::Renderer::Flip::None;

	if (attackable.hasPlayableAnimation())
		renderer.render(attackable.updateAnimation(frameDelta), destRect, flip);
	else if (movable.hasPlayableAnimation())
		renderer.render(movable.updateAnimation(frameDelta), destRect, flip);
	else
		renderer.render(idleAnimation.updateAnimation(frameDelta), destRect, flip);

	// draw debug info
	int width = attackable.hp > 0 ? static_cast<int>(hitbox.w * ((float)attackable.hp / attackable.maxHp)) : 0;
	if (options.renderHealthBars || attackable.hp < attackable.maxHp)
		renderer.drawRectangle({hitbox.x, hitbox.y - 5, width, 2}, {255, 0, 0, 255}, true);
	if (options.renderEntityDrawRectangles)
		renderer.drawRectangle(destRect, {0, 0, 255, 128}, false);
	if (options.renderHitBoxes) {
		renderer.drawRectangle(hitbox, {255, 0, 0, 128}, false);
		renderer.drawRectangle(attackable.getHitbox(movable.getPosition(), flip == sdl::Renderer::Flip::X),
		                       {0, 255, 120, 128}, true);
	}
};
