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
	void leaveCutScene();

private:
	GameData* const mGameData;
	Player* mPlayer;
	sf::Time mTimeSinceStart;
	sf::Vector2f mViewBeforeCutScene;
};

}