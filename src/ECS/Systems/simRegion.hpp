#pragma once

#include "ECS/system.hpp"

namespace ph::system {

class SimRegion : public System
{
public:
	using System::System;

	void update(float dt) override;

private:
	FloatRect mSimRegionCentralPartitionBounds;
	FloatRect mSimRegionBounds;
	bool mInitialized = false;
};

/* NOTE: 
Sim region is area in which entities live inside
the main registry.
*/ 

}
