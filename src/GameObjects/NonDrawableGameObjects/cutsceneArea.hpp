#pragma once

#include "GameObjects/gameObject.hpp"

namespace ph {

class Player;
class GameData;

class CutSceneArea : public GameObject
{
public:
	CutSceneArea(GameData* const gameData, const std::string& cutSceneName, const sf::FloatRect area);

	void updateCurrent(const sf::Time delta) override;
	bool getActivated() const { return mActivated; }

private:
	void createCutScene();

private:
	Player* mPlayer;
	GameData* const mGameData;
	std::string mCutSceneName;
	const sf::FloatRect mArea;
	bool mActivated;
};

}