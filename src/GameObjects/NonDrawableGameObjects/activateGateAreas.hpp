#pragma once 

#include "GameObjects/gameObject.hpp"

namespace ph {

class OpenGateArea : public GameObject
{
public:
	OpenGateArea(const sf::FloatRect area);
	void updateCurrent(const sf::Time delta) override;

	bool getActivated() { return mActivated; }

private:
	const sf::FloatRect mArea;
	bool mActivated;
};

class CloseGateArea : public GameObject
{
public:
	CloseGateArea(const sf::FloatRect area);
	void updateCurrent(const sf::Time delta) override;

	bool getActivated() { return mActivated; }

private:
	const sf::FloatRect mArea;
	bool mActivated;
};

}
