#pragma once

#include "cutScene.hpp"
#include <SFML/System.hpp>
#include <memory>

namespace ph {

class CutSceneManager
{
public:
	CutSceneManager();

	void activateCutscene(std::unique_ptr<CutScene> startingCutScene);

	void updateCutScene(const float dt);

	bool isCutSceneActive();
	bool pausesSystems();

private:
	std::unique_ptr<CutScene> mActiveCutscene;
};

}
