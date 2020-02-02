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
	CutScenesActivating(entt::registry&, CutSceneManager&, AIManager&, SceneManager&);

	void update(float dt) override;

private:
	void activateCutscene(const std::string& name) const;

private:
	CutSceneManager& mCutSceneManager;
	AIManager& mAIManager;
	SceneManager& mSceneManager;
};

}
