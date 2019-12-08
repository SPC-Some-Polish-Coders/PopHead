#pragma once

#include "Utilities/rect.hpp"
#include <string>
#include <vector>
#include <map>
#include <utility>

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
