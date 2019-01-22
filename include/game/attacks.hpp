#include "game/constants.hpp"
#include "game/movement.hpp"

namespace game {
struct Projectile {
	Projectile(Position p, Rectangle h, int d, bool noClip, Velocity v)
	    : position(p), hitBox(h), damage(d), noClip(noClip), v(v)
	{
	}

	Position position;
	Rectangle hitBox;
	int damage;
	bool noClip;
	Velocity v;
};

struct ProjectileBlueprint {
	Rectangle hitBox;
	int damage;
	bool noClip;
	Speed maxSpeed;
	Position startPosition;
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
	ProjectileBlueprint projectileBlueprint;
};
} // namespace game
