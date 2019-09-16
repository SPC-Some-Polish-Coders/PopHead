#pragma once

#include "GameObjects/gameObject.hpp"

namespace ph {

class Player;

class CutSceneArea : public GameObject
{
public:
	CutSceneArea(const std::string& cutSceneName, const sf::FloatRect area);

	void updateCurrent(const sf::Time delta) override;
	bool getActivated() const { return mActivated; }

private:
	void createCutScene();

private:
	Player* mPlayer;
	std::string mCutSceneName;
	const sf::FloatRect mArea;
	bool mActivated;
};

}