#pragma once

#include "ECS/system.hpp"

namespace ph::system {

class Platforms : public System
{
public:
	using System::System;

	void update(float dt) override;

	inline static const float sBodyCircleRadiusAdditionForPlatforms = 2.f;
};

}
