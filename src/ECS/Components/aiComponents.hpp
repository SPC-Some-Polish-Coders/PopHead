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
	};

}
