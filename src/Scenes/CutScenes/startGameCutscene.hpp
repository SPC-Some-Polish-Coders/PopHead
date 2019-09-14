#pragma once

#include "Scenes/cutScene.hpp"
#include <SFML/System.hpp>
#include "GameObjects/DrawableGameObjects/Characters/npc.hpp"

namespace ph {

class Camera;
class SoundPlayer;
class MusicPlayer;
class GUI;
class Car;
class GameData;

class StartGameCutScene : public CutScene
{
public:
	StartGameCutScene(GameObject& root, Camera&, SoundPlayer&, MusicPlayer&, GUI& gui, GameData* const);

	void update(const sf::Time delta) override;

private:
	void createZombie(const sf::Vector2f position);
	void closeCutScene();

private:
	Camera& mCamera;
	SoundPlayer& mSoundPlayer;
	MusicPlayer& mMusicPlayer;
	GUI& mGui;
	GameData* const mGameData;
	float mCutsceneTimeInSeconds;
	bool mHasStartedToSlowDown;
	bool mHasChangedTheMusicToMenuTheme;
	bool mWasPlayerCreated;
	bool mHasPlayerTurnedToNpc;
	bool mHasBilbordFallenOver;
	bool mWereZombieSpawned;
	bool mHasChangedMusicToZombieAttackTheme;
};

}