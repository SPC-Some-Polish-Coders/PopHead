#include "animationStatesResources.hpp"
#include "Utilities/xml.hpp"
#include "Logs/logs.hpp"
#include <algorithm>

namespace ph {

static std::map<std::string, AnimationStatesData> allAnimationsStateData;

void loadAnimationStatesFromFile(const std::string& filepath)
{
	if(allAnimationsStateData.find(filepath) != allAnimationsStateData.end())
		return;

	AnimationStatesData animation;

	Xml animationStatesFile;
	animationStatesFile.loadFromFile("resources/" + filepath);
	const auto statesData = animationStatesFile.getChildren("state");
	for(const auto& data : statesData)
	{
		StateData state;
		state.startFrame = IntRect(
			data.getAttribute("startFrameX")->toUnsigned(),
			data.getAttribute("startFrameY")->toUnsigned(),
			data.getAttribute("startFrameWidth")->toUnsigned(),
			data.getAttribute("startFrameHeight")->toUnsigned()
		);
		state.frameCount = data.getAttribute("frameCount")->toUnsigned();
		const std::string stateName = data.getAttribute("name")->toString();
		animation[stateName] = state;
	}
	 
	allAnimationsStateData[filepath] = animation;
}

AnimationStatesData* getAnimationStates(const std::string& filepath)
{
	auto found = allAnimationsStateData.find(filepath);
	PH_ASSERT_CRITICAL(found != allAnimationsStateData.end(), "Animation states data \"" + filepath + "\" not found!");
	return &found->second;
}

}
