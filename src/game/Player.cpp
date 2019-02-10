#include "game/Player.hpp"

using namespace game;

Player::Player(sdl::Animation idleAnimation, sdl::Animation walkingAnimation, sdl::Animation airUpAnimation,
               sdl::Animation airDownAnimation, sdl::Animation deathAnimation, sdl::Animation hurtAnimation,
               sdl::Animation slideAnimation, std::vector<Attack> attacks)
    : movable(hitbox, 100, walkingAnimation, airUpAnimation, airDownAnimation, {0, 0}, 1),
      attackable(100, 0, attacks, deathAnimation, hurtAnimation, std::chrono::milliseconds(1000)),
      idleAnimation(idleAnimation), slideAnimation(slideAnimation)
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

	if (isSliding)
		renderer.render(slideAnimation.updateAnimation(frameDelta), destRect, flip);
	else if (attackable.hasPlayableAnimation())
		renderer.render(attackable.updateAnimation(frameDelta), destRect, flip);
	else if (movable.hasPlayableAnimation())
		renderer.render(movable.updateAnimation(frameDelta), destRect, flip);
	else
		renderer.render(idleAnimation.updateAnimation(frameDelta), destRect, flip);

	// render projectiles
	for (auto &projectile : attackable.projectiles) {
		sdl::Renderer::Flip f = sdl::Renderer::Flip::None;
		if (projectile.v.x < 0)
			f = sdl::Renderer::Flip::None;

		renderer.render(projectile.updateAnimation(frameDelta), projectile.calcPositionedHitbox());
	}

	// draw texture box
	if (options.renderEntityDrawRectangles)
		renderer.drawRectangle(destRect, {0, 0, 255, 128}, false);

	// draw hit/collision box
	if (options.renderHitBoxes) {
		renderer.drawRectangle(calcPositionedHitbox(), {255, 0, 0, 128}, false);
		renderer.drawRectangle(getAttackHitbox(), {0, 255, 255, 128});
	}
	// render healthbar
	int width = attackable.hp > 0 ? static_cast<int>(hitbox.w * ((float)attackable.hp / attackable.maxHp)) : 0;
	renderer.drawRectangle({movable.calcPositionedHitbox().x, movable.calcPositionedHitbox().y - 2, width, 2},
	                       {255, 0, 0, 255}, true);
}

void Player::startMoving()
{
	movable.startMoving();
}

void Player::stopMoving()
{
	if (movable.getIsMoving()) {
		idleAnimation.reset();
		movable.stopMoving();
	}
}

void Player::moveLeft()
{
	if (hasControl())
		movable.moveLeft();
}
void Player::moveRight()
{
	if (hasControl())
		movable.moveRight();
}
void Player::jump()
{
	if (hasControl())
		movable.jump();
}
void Player::fall()
{
	if (hasControl())
		movable.fall();
}

void Player::slide()
{
	if (!hasControl() || !movable.grounded)
		return;

	slideAnimation.reset();
	attackable.setInvulnerable();
	isSliding = true;
}

void Player::attack()
{
	if (!hasControl())
		return;

	if (attackable.getTimeSinceLastAttack() < comboTimer)
		comboCount++;
	else
		comboCount = 0;
	comboCount %= 3;
	attackable.attack(comboCount);
}

void Player::shadeBlast()
{
	if (!hasControl())
		return;

	if (std::none_of(inventory.cbegin(), inventory.cend(), [](const Item &i) { return i.name == "ShadePotion"; }))
		return;

	comboCount = 0;
	attackable.attack(3);
}

void Player::singleSlam()
{
	if (!hasControl())
		return;

	if (std::none_of(inventory.cbegin(), inventory.cend(), [](const Item &i) { return i.name == "GolemPotion"; }))
		return;

	comboCount = 0;
	attackable.attack(4);
}

void Player::doubleSlam()
{
	if (!hasControl())
		return;

	if (std::none_of(inventory.cbegin(), inventory.cend(), [](const Item &i) { return i.name == "YetiPotion"; }))
		return;

	comboCount = 0;
	attackable.attack(5);
}

void Player::update(sdl::GameClock::duration gameFrameDelta)
{
	if (isSliding && slideAnimation.getLoopCount() > 0) {
		isSliding = false;
		attackable.resetInvulnerable();
	}

	int slideSpeed = 1.5 * movable.maxSpeed;
	if (isSliding) {
		if (movable.getDirection().x < 0)
			movable.v.x = -slideSpeed;
		else
			movable.v.x = slideSpeed;
	}

	movable.update(gameFrameDelta);

	attackable.update(gameFrameDelta);
}

Rectangle Player::getAttackHitbox()
{
	return attackable.getHitbox(movable.getPosition(), movable.getDirection().x < 0);
}

void Player::addXp(int toAdd)
{
	xp += toAdd;
	if (xp > levelThreshold) {
		std::cout << "level up" << std::endl;
		xp -= levelThreshold;
		level++;
		attackable.maxHp = 30 + 20 * level;
		attackable.hp = attackable.maxHp;
	}
}

void Player::setLeveling(int l, int x)
{
	level = l;
	xp = x;
	attackable.maxHp = 30 + 20 * level;
	attackable.hp = attackable.maxHp;
}
