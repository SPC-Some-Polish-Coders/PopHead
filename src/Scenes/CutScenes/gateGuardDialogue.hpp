#pragma once

#include "Scenes/cutScene.hpp"

namespace ph {

class GameData;
class Player;

class GateGuardDialogue : public CutScene
{
public:
	GateGuardDialogue(GameData* const gameData);

	void update(const sf::Time delta) override;

private:
	void initGui();
	void leaveCutScene();

private:
	GameData* const mGameData;
	Player* mPlayer;
	sf::Clock mTimeSinceLastSkipPress;
	int mTimesPressedSkip;
	bool mPlayerOnThePosition;
};

}