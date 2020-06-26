#pragma once

namespace ph {

struct StateData
{
	IntRect startFrame;
	u16 frameCount;
};

using AnimationStatesData = std::map<std::string, StateData>;

void loadAnimationStatesFromFile(const std::string& filepath);
AnimationStatesData* getAnimationStates(const std::string& filepath);

}
