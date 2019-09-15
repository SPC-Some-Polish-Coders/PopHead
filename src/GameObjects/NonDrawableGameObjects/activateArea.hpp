#pragma once 

#include "GameObjects/gameObject.hpp"

namespace ph {

class Player;

class ActivateArea : public GameObject
{
public:
	ActivateArea(const std::string& areaName, const sf::FloatRect area);

	void updateCurrent(const sf::Time delta) override;
	bool getActivated() const { return mActivated; }

private:
	Player* mPlayer;
	const sf::FloatRect mArea;
	bool mActivated;
};

}
