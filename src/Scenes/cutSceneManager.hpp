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

	void updateCutScene(const sf::Time dt);

	bool isCutSceneActive();

private:
	std::unique_ptr<CutScene> mActiveCutscene;
};

}
