#pragma once

#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>

namespace ph::component {
	
	struct Health
	{
		int healthPoints;
		int maxHealthPoints;
	};

	struct Damage
	{
		int damageDealt;
	};
	
	struct Player
	{
	};

	struct Killable
	{

	};

	struct FaceDirection
	{
		sf::Vector2f direction;
	};

	struct MeleeAttacker
	{
		float minSecondsInterval;
		bool isTryingToAttack;
	};

	struct GunAttacker
	{
		float minSecondsInterval;
		float cooldownSinceLastShoot;
		unsigned bullets;
		bool isTryingToAttack;
	};

	struct TaggedToDestroy
	{

	};

	struct DamageTag
	{
		int amountOfDamage;
	};

	struct CollisionWithPlayer
	{
		bool isCollision;
	};

	struct Lifetime
	{
		float lifetime;
	};
}
