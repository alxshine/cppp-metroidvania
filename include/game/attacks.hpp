#include <optional>

#include "game/constants.hpp"
#include "game/movement.hpp"
#include "game/physics.hpp"
#include "game/room_helpers.hpp"

namespace game {
struct Projectile {
	Projectile(Position p, Rectangle h, int d, bool noClip, Velocity v, sdl::Animation animation)
	    : position(p), hitbox(h), damage(d), noClip(noClip), v(v), animation(std::move(animation))
	{
	}

	Rectangle calcPositionedHitbox();
	bool update(sdl::GameClock::duration frameDelta, Rectangle playerHitbox, const CollisionMap &collisionMap);
	sdl::Sprite updateAnimation(sdl::GameClock::duration frameDelta);

	Position position;
	Rectangle hitbox;
	int damage;
	bool noClip;
	Velocity v;
	bool done = false;

	sdl::Animation animation;
};

struct ProjectileBlueprint {
	Rectangle hitBox;
	int damage;
	bool noClip;
	Speed maxSpeed;
	Position startPosition;

  sdl::Animation animation;
};

struct Attack {
	using Type = game_definitions::Attack::Type;

	Attack(Rectangle hitBox, sdl::Animation animation, int damage, std::vector<int> damageFrames,
	       ProjectileBlueprint projectileBlueprint)
	    : hitBox(hitBox), animation(std::move(animation)), damage(damage), damageFrames(std::move(damageFrames)),
	      type(Type::Ranged), projectileBlueprint(std::move(projectileBlueprint))
	{
	}

	Attack(Rectangle hitBox, sdl::Animation animation, int damage, std::vector<int> damageFrames)
	    : hitBox(hitBox), animation(std::move(animation)), damage(damage), damageFrames(std::move(damageFrames)),
	      type(Type::Melee)
	{
	}

	Rectangle hitBox;
	sdl::Animation animation;
	int damage;
	std::vector<int> damageFrames;
	Type type;
  std::optional<ProjectileBlueprint> projectileBlueprint;
};
} // namespace game
