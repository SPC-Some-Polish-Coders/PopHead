#pragma once 
 
#include "Scenes/cutScene.hpp"

namespace ph { 

class SceneManager;

class SubtitlesBeforeStartGameCutscene : public CutScene
{
public:
	SubtitlesBeforeStartGameCutscene(GameObject& root, SceneManager&);

	void update(sf::Time delta) override;

private:
	SceneManager& mSceneManager;
};

} 
