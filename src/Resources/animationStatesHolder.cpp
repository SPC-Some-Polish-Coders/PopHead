#include "animationStatesResources.hpp"
#include "Utilities/xml.hpp"
#include "Logs/logs.hpp"
#include <algorithm>

namespace ph {

static std::map<std::string, AnimationStatesData> allAnimationsStateData;

void loadStatesFromFile(const std::string& filepath)
{
	if(allAnimationsStateData.find(filepath) != allAnimationsStateData.end())
		return;

	AnimationStatesData animation;

	Xml animationStatesFile;
	animationStatesFile.loadFromFile(filepath);
	const auto statesData = animationStatesFile.getChildren("state");
	for(const auto& data : statesData)
	{
		StateData state;
		state.startFrame = IntRect(
			data.getAttribute("startFrameX").toInt(),
			data.getAttribute("startFrameY").toInt(),
			data.getAttribute("startFrameWidth").toInt(),
			data.getAttribute("startFrameHeight").toInt()
		);
		state.framesCount = data.getAttribute("frameCount").toUnsigned();
		const std::string stateName = data.getAttribute("name").toString();
		animation[stateName] = state;
	}
}

AnimationStatesData* getAnimationStates(const std::string& filepath)
{
	auto found = allAnimationsStateData.find(filepath);
	PH_ASSERT_CRITICAL(found == allAnimationsStateData.end(), "Animation states data " + filepath + " not found!");
	return &found->second;
}

}
