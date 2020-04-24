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
		constexpr static float sneakingSpeed = 10.f;
		constexpr static float attackingSpeed = 70.f;
		constexpr static float afterAttackSpeed = 10.f;

		constexpr static float farDistance = 200.f;
		constexpr static float closeDistance = 50.f;

		constexpr static float coolDownTime = 2.f;

		float coolDownTimer = 0.f;
	};
}
