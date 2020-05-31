#pragma once

#include "ECS/system.hpp"

namespace ph::system {

class DebugVisualization : public System
{
public:
	using System::System;

	void update(float dt) override;
};

}
