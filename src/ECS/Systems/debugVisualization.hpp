#pragma once

#include "ECS/system.hpp"

namespace ph::system {

class DebugVisualization : public System
{
public:
	using System::System;

	void update(float dt) override;

	static void setSimRegionBoundsPtr(FloatRect* ptr) { simRegionRect = ptr; };
	static void setSimRegionCentralPartitionBoundsPtr(FloatRect* ptr) { simRegionCentralPartitionRect = ptr; };
private:
	inline static FloatRect* simRegionRect;
	inline static FloatRect* simRegionCentralPartitionRect;
};

}
