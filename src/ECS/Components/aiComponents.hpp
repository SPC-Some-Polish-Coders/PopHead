#pragma once

#include "AI/aiManager.hpp"
#include <vector>

namespace ph::component {

	struct Zombie
	{
		PathMode pathMode;
		sf::Vector2f currentDirectionVector;
		float timeFromStartingThisMove = 0.f;
		float timeFromLastGrowl;
		float timeToMoveToAnotherTile;
	};

	struct SlowZombieBehavior
	{
		constexpr static float farFromPlayerSpeed = 50.f;
		constexpr static float sneakingSpeed = 50.f;
		constexpr static float attackingSpeed = 50.f;
		constexpr static float afterAttackSpeed = 50.f;

		constexpr static float farDistance = 60.f;
		constexpr static float closeDistance = 30.f;

		float timeFromLastAttack = 0.f;
	};
}
