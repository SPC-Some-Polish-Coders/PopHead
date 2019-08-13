#pragma once

#include "Scenes/cutScene.hpp"
#include <SFML/System.hpp>

namespace ph {

class Camera;
class SoundPlayer;
class GUI;
class Car;

class StartGameCutScene : public CutScene
{
public:
	StartGameCutScene(GameObject& root, Camera&, SoundPlayer&, GUI& gui);
	void update(const sf::Time delta) override;

private:
	void updateGui(const float cutsceneTimeInSeconds, Car& car);

private:
	Camera& mCamera;
	SoundPlayer& mSoundPlayer;
	GUI& mGui;
	sf::Clock mClock;
	bool mHasStartedToSlowDown;
};

}