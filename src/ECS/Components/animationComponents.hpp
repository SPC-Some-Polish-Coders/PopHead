#pragma once

#include "Resources/animationStatesResources.hpp"
#include "Utilities/rect.hpp"
#include <SFML/System/Time.hpp>
#include <map>
#include <string>
#include <vector>

namespace ph::component {

struct AnimationData
{
	std::string currentStateName;
	AnimationStatesData* states;
	float delay = 0.1f;
	float elapsedTime = 0.f;
	std::size_t currentFrameIndex = 0;
	bool isPlaying = false;
};

}
