#pragma once

#include "Scenes/cutScene.hpp"

namespace ph {

class GUI;
class SceneManager;

class MovementContolsGuide : public CutScene
{
public:
	MovementContolsGuide(GUI&, SceneManager&);

	void update(const sf::Time dt);

private:
	void closeCutscene();

private:
	sf::Clock mTimeSinceLastSkipPress;
	GUI& mGui;
	SceneManager& mSceneManager;
	unsigned int mTimesPressedSkip;
};

}
