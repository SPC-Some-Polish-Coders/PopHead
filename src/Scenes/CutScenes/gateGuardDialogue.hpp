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
	void leaveCutScene();
	void initGui();

private:
	GameData* const mGameData;
	Player* mPlayer;
	sf::Vector2f mViewBeforeCutScene;
	sf::Clock mTimeSinceLastSkipPress;
	int mTimesPressedSkip;
	bool mPlayerOnThePosition;
};

}