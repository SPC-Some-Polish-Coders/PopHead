#include "pch.hpp"
#include "animationStatesResources.hpp"

namespace ph {

static std::map<std::string, AnimationStatesData> allAnimationsStateData;

void loadAnimationStatesFromFile(const std::string& filepath)
{
	if(allAnimationsStateData.find(filepath) != allAnimationsStateData.end())
		return;

	AnimationStatesData animation;

	Xml animationStatesFile;
	PH_ASSERT_CRITICAL(animationStatesFile.loadFromFile("resources/" + filepath),
		"animation states file \"resources/" + filepath + "\" wasn't loaded correctly!");
	const auto statesData = animationStatesFile.getChildren("state");
	for(const auto& data : statesData)
	{
		StateData state;
		state.startFrame = IntRect(
			data.getAttribute("startFrameX")->toI32(),
			data.getAttribute("startFrameY")->toI32(),
			data.getAttribute("startFrameWidth")->toI32(),
			data.getAttribute("startFrameHeight")->toI32()
		);
		state.frameCount = data.getAttribute("frameCount")->toU32();
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
