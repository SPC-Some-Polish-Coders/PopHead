#pragma once

#include "Utilities/rect.hpp"

namespace ph::component {

	struct Area
	{
		FloatRect areaBody;
	};

	struct AreaVelocityChangingEffect
	{
		float areaSpeedMultiplier;
	};

	struct Entrance
	{
		std::string entranceDestination;
	};

}
