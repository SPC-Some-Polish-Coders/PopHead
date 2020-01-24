#pragma once

#include "ECS/system.hpp"

namespace ph {

class CutSceneManager;
class GUI;
class AIManager;
class SceneManager;

}

namespace ph::system {

class CutScenesActivating : public System
{
public:
	CutScenesActivating(entt::registry&, CutSceneManager&, GUI&, AIManager&, SceneManager&);

	void update(float dt) override;

private:
	void activateCutscene(const std::string& name) const;

private:
	CutSceneManager& mCutSceneManager;
	GUI& mGui;
	AIManager& mAIManager;
	SceneManager& mSceneManager;
};

}
