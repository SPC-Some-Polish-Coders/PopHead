#pragma once

#include <SFML/System/Vector2.hpp>

namespace ph::component {
	
	struct Health
	{
		i32 healthPoints;
		i32 maxHealthPoints;
	};

	struct Damage
	{
		i32 damageDealt;
	};
	
	struct Player {};

	struct Killable {};

	struct InPlayerGunAttackArea {};

	struct FaceDirection : public Vec2 
	{
		using Vec2::operator=;
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
		i32 amountOfDamage;
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
		Vec2 startingShotPos;
		Vec2 endingShotPos;
	};

	struct CurrentGun {}; // @no-debugger

	struct CurrentMeleeWeapon {}; // @no-debugger

	struct DamageAnimation // @no-debugger
	{
		float timeToEndColorChange;
		bool animationStarted = false;
	};

}
