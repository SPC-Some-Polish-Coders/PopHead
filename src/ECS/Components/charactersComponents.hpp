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
		operator sf::Vector2f&() { return *this; }
		FaceDirection& operator=(sf::Vector2f v) { x = v.x; y = v.y; return *this; }
	};

	struct GunAttacker
	{
		bool isTryingToAttack;
		float timeBeforeHiding;
		float timeToHide;
	};

	struct DeadCharacter
	{
		float timeToFadeOut = 0.f;
		float timeFromDeath = 0.f;
	};

	struct TaggedToDestroy {};

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

	struct CurrentGun {};

	struct CurrentMeleeWeapon {};

	struct DamageAnimation
	{
		float timeToEndColorChange;
		bool animationStarted = false;
	};

}
