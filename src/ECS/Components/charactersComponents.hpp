#pragma once

#include "Utilities/rect.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <set>

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
		float cooldownSinceLastHit;
		bool isTryingToAttack;
	};

	struct GunAttacker
	{
		float minSecondsInterval;
		float cooldownSinceLastShoot;
		unsigned bullets;
		bool isTryingToAttack;
		bool canAttack;
	};

	struct FadingOut
	{
		float timeFromDeath = 0.f;
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

	struct LastingShot
	{
		sf::Vector2f startingShotPos;
		sf::Vector2f endingShotPos;
	};

	struct PlayerGun
	{
		float timeBeforeHiding;
		float cooldownSinceLastShot;
	};

	struct IsInArea
	{
		std::vector<FloatRect> areas;
	};

	struct VelocityEffects
	{
		std::multiset<float> velocityMultipliers;
	};

	struct DamageAnimation
	{
		float timeToEnd;
	};
}
