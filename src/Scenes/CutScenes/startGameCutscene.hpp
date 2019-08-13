#pragma once

#include "Scenes/cutScene.hpp"
#include <SFML/System.hpp>

namespace ph {

class Camera;
class SoundPlayer;
class MusicPlayer;
class GUI;
class Car;

class StartGameCutScene : public CutScene
{
public:
	StartGameCutScene(GameObject& root, Camera&, SoundPlayer&, MusicPlayer&, GUI& gui);
	void update(const sf::Time delta) override;

private:
	void updateGui(const float cutsceneTimeInSeconds, Car& car);

private:
	Camera& mCamera;
	SoundPlayer& mSoundPlayer;
	MusicPlayer& mMusicPlayer;
	GUI& mGui;
	sf::Clock mClock;
	bool mHasStartedToSlowDown;
	bool mHasChangedTheMusic;
};

}