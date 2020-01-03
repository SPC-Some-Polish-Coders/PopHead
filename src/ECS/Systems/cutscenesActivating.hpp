#pragma once

#include "ECS/system.hpp"

namespace ph {

class CutSceneManager;
class GUI;
class MusicPlayer;
class SoundPlayer;
class AIManager;
class SceneManager;

}

namespace ph::system {

class CutScenesActivating : public System
{
public:
	CutScenesActivating(entt::registry&, CutSceneManager&, GUI&, MusicPlayer&, SoundPlayer&, AIManager&, SceneManager&);

	void update(float dt) override;

private:
	void activateCutscene(const std::string& name) const;

private:
	CutSceneManager& mCutSceneManager;
	GUI& mGui;
	MusicPlayer& mMusicPlayer;
	SoundPlayer& mSoundPlayer;
	AIManager& mAIManager;
	SceneManager& mSceneManager;
};

}
