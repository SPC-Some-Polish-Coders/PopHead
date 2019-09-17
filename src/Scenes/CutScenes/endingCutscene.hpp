#pragma once

#include "Scenes/cutScene.hpp"

namespace ph {

class GUI;
class MusicPlayer;
class Camera;

class EndingCutScene : public CutScene
{
public:
	EndingCutScene(GameObject& root, GUI&, MusicPlayer&, Camera&);

	void update(const sf::Time delta) override;

private:
	GUI& mGui;
	MusicPlayer& mMusicPlayer;
	Camera& mCamera;
};

}