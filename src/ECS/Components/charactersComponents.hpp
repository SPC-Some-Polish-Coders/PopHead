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

	struct InPlayerGunAttackArea
	{
	};

	struct FaceDirection
	{
		sf::Vector2f direction;
	};

	struct GunAttacker
	{
		unsigned bullets;
		bool isTryingToAttack;
		float timeBeforeHiding;
		float timeToHide;
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
		float pushForce;
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
	};

	struct CurrentMeleeWeapon
	{
	};

	struct DamageAnimation
	{
		float timeToEndColorChange;
		bool animationStarted = false;
	};

}
