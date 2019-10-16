#pragma once

#include "Scenes/cutScene.hpp"

namespace ph {

class GUI;
class SceneManager;

class FightControlsGuide : public CutScene
{
public:
	FightControlsGuide(GameObject& root, GUI&, SceneManager&);

	void update(const sf::Time delta);

private:
	void closeCutscene();

private:
	sf::Clock mTimeSinceLastSkipPress;
	GUI& mGui;
	SceneManager& mSceneManager;
	unsigned int mTimesPressedSkip;
};

}
