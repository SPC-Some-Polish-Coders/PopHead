#pragma once

#include "Scenes/cutScene.hpp"
#include <SFML/System.hpp>

namespace ph {

class Camera;
class SoundPlayer;

class StartGameCutScene : public CutScene
{
public:
	StartGameCutScene(GameObject& root, Camera&, SoundPlayer&);
	void update(const sf::Time delta) override;

private:
	Camera& mCamera;
	SoundPlayer& mSoundPlayer;
	sf::Clock mClock;
	bool mHasStartedToSlowDown;
};

}