#pragma once

#include "Resources/animationStatesResources.hpp"

namespace ph::component {

struct AnimationData // @no-debugger
{
	std::string currentStateName;
	AnimationStatesData* states;
	float delay = 0.1f;
	float elapsedTime = 0.f;
	u16 currentFrameIndex = 0;
	bool isPlaying = false;
};

}
