#pragma once

#include "pch.hpp"

namespace ph {

struct StateData
{
	IntRect startFrame;
	unsigned frameCount;
};

using AnimationStatesData = std::map<std::string, StateData>;

void loadAnimationStatesFromFile(const std::string& filepath);
AnimationStatesData* getAnimationStates(const std::string& filepath);

}
