#pragma once

#include "Utilities/rect.hpp"

namespace ph::component {

	struct Area
	{
		FloatRect areaBody;
	};

	struct VelocityChangingEffect
	{
		float speedMultiplier;
	};

}
