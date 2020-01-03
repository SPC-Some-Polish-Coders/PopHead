#pragma once

#include <string>

namespace ph::component {

struct CutScene
{
	std::string name;
	bool isStartingCutSceneOnThisMap;
	bool wasActivated = false;
};

}
