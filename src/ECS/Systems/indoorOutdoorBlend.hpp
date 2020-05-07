#pragma once

#include "ECS/system.hpp"

namespace ph::system {

class IndoorOutdoorBlend : public System
{
public:
	using System::System;

	void update(float dt) override;

private:
	float mPlayerOutdoor = 1.f;
};

}
