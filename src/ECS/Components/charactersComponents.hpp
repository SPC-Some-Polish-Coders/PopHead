#pragma once

#include "Utilities/rect.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <set>
#include <vector>

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
		float cooldownSinceLastHit;
		bool isTryingToAttack;
		bool canAttack;
		bool isAttacking;
	};

	struct GunAttacker
	{
		float cooldownSinceLastShoot;
		unsigned bullets;
		bool isTryingToAttack;
		bool canAttack;
	};

	struct TimeToFadeOut
	{
		float seconds = 0.f;
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

	struct CurrentGun
	{
		float timeBeforeHiding;
		float cooldownSinceLastShot;
	};

	struct CurrentMeleeWeapon
	{
		float rotation = 0.f;
	};

	struct IsInArea
	{
	};

	struct DamageAnimation
	{
		float timeToEndColorChange;
		bool wasAnimationStarted = false;
	};

}
