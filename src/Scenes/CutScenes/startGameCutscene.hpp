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
	void updateGui(const float cutsceneTimeInSeconds, Car& car);

private:
	Camera& mCamera;
	SoundPlayer& mSoundPlayer;
	MusicPlayer& mMusicPlayer;
	GUI& mGui;
	GameData* const mGameData;
	sf::Clock mClock;
	bool mHasStartedToSlowDown;
	bool mHasChangedTheMusic;
	bool mWasNpcCreated;
	bool mHasPlayerTurnedToNpc;
};

}