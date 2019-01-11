#include "game/Mob.hpp"
game::Mob::Mob(const Mob &rhs)
    : name(rhs.name), movable(rhs.movable), attackable(rhs.attackable), hitbox(rhs.hitbox), renderSize(rhs.renderSize),
      walkingAnimation(rhs.walkingAnimation), idleAnimation(std::make_unique<sdl::Animation>(*rhs.idleAnimation)),
      ai(rhs.ai)
{
}

game::Mob::Mob(const std::string name, Health health, int speedPerSecond, Rectangle hitbox, Rectangle renderSize,
               sdl::Animation walkingAnimation, OptionalAnimation idleAnimation, std::vector<Attack> attacks,
               std::shared_ptr<AI> ai)
    : name(name), movable(speedPerSecond, walkingAnimation), attackable(health, attacks, movable), hitbox(hitbox),
      renderSize(renderSize), walkingAnimation(walkingAnimation), idleAnimation(std::move(idleAnimation)), ai(ai)
{
}

game::Mob::~Mob() {}

game::Rectangle game::Mob::calcRenderTarget() const
{
	// Calculate position, centering horizontally and bottom-aligning vertically
	return {movable.getPosition().x - static_cast<int>(0.5 * renderSize.w), movable.getPosition().y - renderSize.h,
	        renderSize.w, renderSize.h};
}

game::Rectangle game::Mob::calcPositionedHitbox() const
{
	Rectangle destRect = calcRenderTarget(); // Inefficient, for Mob::render(), but maybe good enough
	return {destRect.x + hitbox.x, destRect.y + hitbox.y, hitbox.w, hitbox.h};
}

void game::Mob::performAiStep(const CollisionMap &collisionMap, Rectangle playerHitBox)
{
	ai->controlEntity(movable, attackable, collisionMap, playerHitBox);
}

void game::Mob::render(const sdl::Renderer &renderer, sdl::GameClock::duration frameDelta,
                       const sdl::RenderOptions &options)
{
	Rectangle destRect = calcRenderTarget();
	Rectangle hitbox = calcPositionedHitbox();

	sdl::Renderer::Flip flip;
	auto dir = movable.getDirection();
	if (dir.x < 0)
		flip = sdl::Renderer::Flip::X;
	else
		flip = sdl::Renderer::Flip::None;

	if (movable.hasPlayableAnimation() || idleAnimation == nullptr)
		renderer.render(walkingAnimation.updateAnimation(frameDelta), destRect, flip);
	else
		renderer.render(idleAnimation->updateAnimation(frameDelta), destRect, flip);

	// draw debug info
	if (options.renderHealthBars)
		renderer.drawRectangle(
		    {hitbox.x, hitbox.y - 5, static_cast<int>(hitbox.w * ((float)attackable.hp / attackable.maxHp)), 2},
		    {255, 0, 0, 255}, true);
	if (options.renderEntityDrawRectangles)
		renderer.drawRectangle(destRect, {0, 0, 255, 128}, false);
	if (options.renderHitBoxes)
		renderer.drawRectangle(hitbox, {255, 0, 0, 128}, false);
};
