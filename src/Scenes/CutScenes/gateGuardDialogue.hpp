#pragma once

#include "Scenes/cutScene.hpp"

namespace ph {

class GUI;

class GateGuardDialogue : public CutScene
{
public:
	GateGuardDialogue(GUI& gui);

	void update(const sf::Time dt) override;

private:
	void initGui();
	void leaveCutScene();

private:
	GUI& mGui;
	sf::Clock mTimeSinceLastSkipPress;
	int mTimesPressedSkip;
	bool mPlayerOnThePosition;
};

}