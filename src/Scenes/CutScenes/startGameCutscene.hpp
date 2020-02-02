//#pragma once
//
//#include "Scenes/cutScene.hpp"
//#include <SFML/System.hpp>
//#include <entt/entt.hpp>
//
//namespace ph {
//
//class Camera;
//class SoundPlayer;
//class MusicPlayer;
//class GUI;
//class SceneManager;
//
//class StartGameCutScene : public CutScene
//{
//public:
//	StartGameCutScene(entt::registry& gameRegistry, SoundPlayer&, MusicPlayer&, GUI&, SceneManager&);
//
//	void update(const sf::Time dt) override;
//
//private:
//	void closeCutScene();
//
//private:
//	entt::registry& mRegistry;
//	SoundPlayer& mSoundPlayer;
//	MusicPlayer& mMusicPlayer;
//	GUI& mGui;
//	SceneManager& mSceneManager;
//	float mCutsceneTimeInSeconds;
//	bool mWasGuiHidden;
//	bool mHasStartedToSlowDown;
//	bool mWasPlayerCreated;
//};
//
//}