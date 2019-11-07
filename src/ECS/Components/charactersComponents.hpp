#pragma once

#include <SFML/System/Clock.hpp>

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

	struct MeleeAttacker
	{
		float minSecondsInterval;
		bool isTryingToAttack;
	};

	struct GunAttacker
	{
		float minSecondsInterval;
		unsigned bullets;
		bool isTryingToAttack;
	};

	struct AttackDelayClock
	{
		sf::Clock delayClock;
	};

	struct TaggedToDestroy
	{

	};
}
