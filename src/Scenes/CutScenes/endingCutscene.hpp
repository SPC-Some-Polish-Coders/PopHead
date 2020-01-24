//#pragma once
//
//#include "Scenes/cutScene.hpp"
//#include "entt/entt.hpp"
//
//namespace ph {
//
//class GUI;
//class MusicPlayer;
//class AIManager;
//class SceneManager;
//
//class EndingCutScene : public CutScene
//{
//public:
//	EndingCutScene(entt::registry&, GUI&, MusicPlayer&, AIManager&, SceneManager&);
//
//	void update(const sf::Time dt) override;
//
//private:
//	void initGui();
//	void leaveCutScene();
//
//private:
//	entt::registry& mRegistry;
//	GUI& mGui;
//	MusicPlayer& mMusicPlayer;
//	AIManager& mAIManager;
//	SceneManager& mSceneManager;
//	sf::Clock mTimeSinceLastSkipPress;
//	int mTimesPressedSkip;
//};
//
//}