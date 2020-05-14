#pragma once

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
	
	struct Player {};

	struct Killable {};

	struct InPlayerGunAttackArea {};

	struct FaceDirection : public sf::Vector2f 
	{
		using sf::Vector2f::operator=;
	};

	struct GunAttacker // @no-debugger
	{
		bool isTryingToAttack;
		float timeBeforeHiding;
		float timeToHide;
	};

	struct DeadCharacter // @no-debugger
	{
		float timeToFadeOut = 0.f;
		float timeFromDeath = 0.f;
	};

	struct TaggedToDestroy {};

	struct DamageTag // @no-debugger
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

	struct LastingShot // @no-debugger
	{
		sf::Vector2f startingShotPos;
		sf::Vector2f endingShotPos;
	};

	struct CurrentGun {}; // @no-debugger

	struct CurrentMeleeWeapon {}; // @no-debugger

	struct DamageAnimation // @no-debugger
	{
		float timeToEndColorChange;
		bool animationStarted = false;
	};

}
